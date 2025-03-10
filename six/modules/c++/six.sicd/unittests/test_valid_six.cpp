/* =========================================================================
* This file is part of six.sicd-c++
* =========================================================================
*
* (C) Copyright 2004 - 2016, MDA Information Systems LLC
*
* six.sicd-c++ is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this program; If not,
* see <http://www.gnu.org/licenses/>.
*
*/

#include <stdlib.h>

#include <string>
#include <iostream>
#include <string>
#include <utility>
#include <std/filesystem>
#include <std/optional>
#include <cmath>
#include <std/span>

#include <io/FileInputStream.h>
#include <logging/NullLogger.h>
#include <import/sys.h>

#include <import/six.h>
#include <import/six/sicd.h>
#include <six/sicd/SICDByteProvider.h>
#include <six/NITFWriteControl.h>
#include <six/XMLControlFactory.h>
#include <six/sicd/ComplexXMLControl.h>
#include <six/sicd/NITFReadComplexXMLControl.h>
#include <six/sicd/Utilities.h>
#include <six/XmlLite.h>

#include "../tests/TestUtilities.h"
#include "TestCase.h"

#if _MSC_VER
#pragma warning(disable: 4459) //  declaration of '...' hides global declaration
#endif

namespace fs = std::filesystem;

static std::string testName;

constexpr auto PlatformEncoding = sys::Platform == sys::PlatformType::Windows
? xml::lite::StringEncoding::Windows1252
    : xml::lite::StringEncoding::Utf8;

static fs::path argv0()
{
    static const sys::OS os;
    static const fs::path retval = os.getSpecialEnv("0");
    return retval;
}

static fs::path nitfRelativelPath(const fs::path& filename)
{
    return fs::path("six") / "modules" / "c++" / "six" / "tests" / "nitf" / filename;
}

static fs::path getNitfPath(const fs::path& filename)
{
    const auto root_dir = six::testing::buildRootDir(argv0());
    return root_dir / nitfRelativelPath(filename);
}

static fs::path nitfPluginRelativelPath()
{
    if (argv0().filename() == "Test.exe") // Google Test in Visual Studio
    {
        static const sys::OS os;
        static const std::string configuration = os.getSpecialEnv("Configuration");
        static const std::string platform = os.getSpecialEnv("Platform");
        return fs::path("externals") / "nitro" / platform / configuration / "share" / "nitf" / "plugins";
    }

    //return fs::path("install") / "share" / "six.sicd" / "conf" / "schema";
    return fs::path("install") / "share" / "CSM" / "plugins";
}
static void setNitfPluginPath()
{
    const auto path = six::testing::buildRootDir(argv0()) / nitfPluginRelativelPath();
    //std::clog << "NITF_PLUGIN_PATH=" << path << "\n";
    sys::OS().setEnv("NITF_PLUGIN_PATH", path.string(), true /*overwrite*/);
}


static std::shared_ptr<six::Container> getContainer(six::sicd::NITFReadComplexXMLControl& reader)
{
    auto container = reader.getContainer();
    TEST_ASSERT_EQ(six::DataType::COMPLEX, container->getDataType());
    TEST_ASSERT_EQ(static_cast<size_t>(1), container->size());
    return container;
}

static std::unique_ptr<six::sicd::ComplexData> getComplexData(const six::Container& container, size_t jj)
{
    std::unique_ptr<six::Data> data_;
    data_.reset(container.getData(jj)->clone());

    TEST_ASSERT_EQ(six::DataType::COMPLEX, data_->getDataType());
    std::unique_ptr<six::sicd::ComplexData> retval(dynamic_cast<six::sicd::ComplexData*>(data_.release()));

    logging::NullLogger nullLogger;
    //TEST_ASSERT_TRUE(retval->validate(nullLogger));
    const auto& geoData = *(retval->geoData);
    TEST_ASSERT_TRUE(geoData.validate(nullLogger));
    const auto& imageData = *(retval->imageData);
    TEST_ASSERT_TRUE(imageData.validate(geoData, nullLogger));

    const auto& classification = retval->getClassification();
    TEST_ASSERT_TRUE(classification.isUnclassified());

    return retval;
}

static void test_nitf_image_info(six::sicd::ComplexData& complexData, const fs::path& inputPathname,
    nitf::PixelValueType expectedPixelValueType)
{
    constexpr auto expectedBlockingMode = nitf::BlockingMode::Pixel;
    constexpr auto expectedImageRepresentation = nitf::ImageRepresentation::NODISPLY;

    const six::NITFImageInfo nitfImageInfo(&complexData);

    auto pixelValueType = nitfImageInfo.getPixelType();
    TEST_ASSERT_EQ(expectedPixelValueType, pixelValueType);

    auto blockingMode = nitfImageInfo.getBlockingMode();
    TEST_ASSERT_EQ(expectedBlockingMode, blockingMode);

    auto imageRepresentation = nitfImageInfo.getImageRepresentation();
    TEST_ASSERT_EQ(expectedImageRepresentation, imageRepresentation);

    nitf::IOHandle io(inputPathname.string());
    nitf::Reader reader;
    const auto record = reader.read(io);
    for (const auto& recordImage : record.getImages())
    {
        const nitf::ImageSegment imageSegment(recordImage);
        const auto subheader = imageSegment.getSubheader();

        pixelValueType = subheader.pixelValueType();
        TEST_ASSERT_EQ(expectedPixelValueType, pixelValueType);

        blockingMode = subheader.imageBlockingMode();
        TEST_ASSERT_EQ(expectedBlockingMode, blockingMode);

        imageRepresentation = subheader.imageRepresentation();
        TEST_ASSERT_EQ(expectedImageRepresentation, imageRepresentation);
    }
}

static void valid_six_50x50_(const std::vector<std::filesystem::path>* pSchemaPaths)
{
    static const auto inputPathname = getNitfPath("sicd_50x50.nitf");
    std::unique_ptr<six::sicd::ComplexData> pComplexData;
    const auto image = six::sicd::readFromNITF(inputPathname, pSchemaPaths, pComplexData);
    const six::Data* pData = pComplexData.get();

    TEST_ASSERT_EQ(six::PixelType::RE32F_IM32F, pData->getPixelType());
    TEST_ASSERT_EQ(static_cast<size_t>(8), pData->getNumBytesPerPixel());

    const std::string classificationText("NON CLASSIFIE' / UNCLASSIFIED");
    const auto& classification = pData->getClassification();
    const auto actual = classification.getLevel();
    TEST_ASSERT_EQ(actual, classificationText);

    test_nitf_image_info(*pComplexData, inputPathname, nitf::PixelValueType::Floating);
}
TEST_CASE(valid_six_50x50)
{
    setNitfPluginPath();

    valid_six_50x50_(nullptr /*pSchemaPaths*/); // no XML validiaton
  
    const std::vector<std::filesystem::path> schemaPaths;
    valid_six_50x50_(&schemaPaths); // validate against schema
}

const std::string classificationText_iso8859_1("NON CLASSIFI\xc9 / UNCLASSIFIED");  // ISO8859-1 "NON CLASSIFI� / UNCLASSIFIED"
const std::string classificationText_utf_8("NON CLASSIFI\xc3\x89 / UNCLASSIFIED");  // UTF-8 "NON CLASSIFI� / UNCLASSIFIED"

TEST_CASE(sicd_French_xml)
{
    setNitfPluginPath();

    const auto inputPathname = getNitfPath("sicd_French_xml.nitf");
    std::unique_ptr<six::sicd::ComplexData> pComplexData;
    //const std::vector<std::filesystem::path> schemaPaths;
    //const auto image = six::sicd::readFromNITF(inputPathname, &schemaPaths, pComplexData);
    const auto image = six::sicd::readFromNITF(inputPathname, pComplexData); // no validation
    const six::Data* pData = pComplexData.get();

    const auto expectedCassificationText = sys::Platform == sys::PlatformType::Linux ? classificationText_utf_8 : classificationText_iso8859_1;
    const auto& classification = pData->getClassification();
    const auto actual = classification.getLevel();
    TEST_ASSERT_EQ(actual, expectedCassificationText);

    test_nitf_image_info(*pComplexData, inputPathname, nitf::PixelValueType::Floating);
}

static bool find_string(io::FileInputStream& stream, const std::string& s)
{
    const auto pos = stream.tell();

    constexpr sys::Off_T offset = 0x0000558e;
    std::string streamAsString;
    {
        stream.seek(offset, io::Seekable::START);
        io::StringStream stringStream;
        stream.streamTo(stringStream);
        streamAsString = stringStream.stream().str();
    }
    const auto result = streamAsString.find(s);
    if ((result != std::string::npos) && (result == 0))
    {
        stream.seek(offset, io::Seekable::START);
        return true;
    }

    stream.seek(pos, io::Seekable::START);
    return false;
}
static void sicd_French_xml_raw_(bool storeEncoding)
{
    // This is a binary file with XML burried in it somewhere
    const auto path = getNitfPath("sicd_French_xml.nitf");

    io::FileInputStream input(path.string());
    const auto result = find_string(input, "<SICD ");
    TEST_ASSERT_TRUE(result);

    six::MinidomParser xmlParser(storeEncoding);
    xmlParser.parse(input);
    const auto& root = getRootElement(getDocument(xmlParser));
    const auto& classificationXML = root.getElementByTagName("Classification", true /*recurse*/);

    if (storeEncoding)
    {
        const auto encoding = classificationXML.getEncoding();
        TEST_ASSERT(encoding == PlatformEncoding);
    }

    // UTF-8 characters in 50x50.nitf
    std::string expectedCharData;
    size_t expectedLength;
    if (storeEncoding)
    {
        expectedCharData = sys::Platform == sys::PlatformType::Linux ? classificationText_utf_8 : classificationText_iso8859_1;
        expectedLength = expectedCharData.length();
    }
    else
    {
        expectedCharData = sys::Platform == sys::PlatformType::Linux ? std::string() : classificationText_iso8859_1;
        expectedLength = sys::Platform == sys::PlatformType::Linux ? 28 : classificationText_iso8859_1.length();
    }
    const auto characterData = classificationXML.getCharacterData();
    TEST_ASSERT_EQ(characterData.length(), expectedLength);
    if (storeEncoding)
    {
        TEST_ASSERT_EQ(characterData, expectedCharData);
    }
    else
    {
        TEST_ASSERT_EQ(characterData[0], expectedCharData[0]);
    }

    std::u8string u8_expectedCharData8;
    if (storeEncoding)
    {
        u8_expectedCharData8 = str::fromUtf8(classificationText_utf_8);
    }
    else
    {
        u8_expectedCharData8 = sys::Platform == sys::PlatformType::Linux ? std::u8string() : str::fromUtf8(classificationText_utf_8);
    }
    expectedLength = u8_expectedCharData8.length();

    std::u8string u8_characterData;
    classificationXML.getCharacterData(u8_characterData);
    std::clog << "'" << u8_characterData.length() << "' '" << expectedLength << "'\n";
    TEST_ASSERT_EQ(u8_characterData.length(), expectedLength);
    TEST_ASSERT(u8_characterData == u8_expectedCharData8);
}
TEST_CASE(sicd_French_xml_raw)
{
    sicd_French_xml_raw_(true /*storeEncoding*/);
    sicd_French_xml_raw_(false /*storeEncoding*/);
}

static void test_assert(const six::sicd::ComplexData& complexData,
    six::PixelType expectedPixelType, size_t expectedNumBytesPerPixel)
{
    TEST_ASSERT_EQ(expectedPixelType, complexData.getPixelType());

    const auto& classification = complexData.getClassification();
    TEST_ASSERT_TRUE(classification.isUnclassified());

    const auto numBytesPerPixel = complexData.getNumBytesPerPixel();
    TEST_ASSERT_EQ(expectedNumBytesPerPixel, numBytesPerPixel);
}

static std::vector<std::byte> readFromNITF(const fs::path& inputPathname)
{
    std::unique_ptr<six::sicd::ComplexData> pComplexData;
    auto image = six::sicd::readFromNITF(inputPathname, pComplexData);

    test_assert(*pComplexData, six::PixelType::RE32F_IM32F, sizeof(std::complex<float>));

    return image;

}
static std::vector<std::byte> readFromNITF(const fs::path& inputPathname, six::PixelType pixelType)
{
    if (pixelType == six::PixelType::RE32F_IM32F)
    {
        return readFromNITF(inputPathname);
    }
    throw std::invalid_argument("Unknown pixelType");
}

TEST_CASE(test_readFromNITF_sicd_50x50)
{
    setNitfPluginPath();

    auto inputPathname = getNitfPath("sicd_50x50.nitf");
    auto buffer = readFromNITF(inputPathname);
}

static six::sicd::ComplexImageResult readSicd_(const fs::path& sicdPathname,
    six::PixelType expectedPixelType, size_t expectedNumBytesPerPixel)
{
    auto result = six::sicd::Utilities::readSicd(sicdPathname);
    test_assert(*(result.pComplexData), expectedPixelType, expectedNumBytesPerPixel);
    return result;
}
static std::vector<std::complex<float>> readSicd(const fs::path& inputPathname)
{
    return readSicd_(inputPathname, six::PixelType::RE32F_IM32F, sizeof(std::complex<float>)).widebandData;
}
TEST_CASE(test_read_sicd_50x50)
{
    setNitfPluginPath();

    auto inputPathname = getNitfPath("sicd_50x50.nitf");
    auto widebandData = readSicd(inputPathname);
}

static std::vector<std::complex<float>> make_complex_image(const six::sicd::ComplexData& complexData, const types::RowCol<size_t>& dims)
{
    if (complexData.getPixelType() == six::PixelType::RE32F_IM32F)
    {
        return six::sicd::testing::make_complex_image(dims);
    }
    throw std::invalid_argument("Unknown pixelType");
}

template<typename T>
static void test_assert_eq(std::span<const std::byte> bytes, const std::vector<T>& rawData)
{
    const auto rawDataSizeInBytes = rawData.size() * sizeof(rawData[0]);
    TEST_ASSERT_EQ(bytes.size(), rawDataSizeInBytes);

    const auto rawDataBytes = six::as_bytes(rawData);
    TEST_ASSERT_EQ(bytes.size(), rawDataBytes.size());
    for (size_t i = 0; i < bytes.size(); i++)
    {
        const auto bytes_i = static_cast<uint8_t>(bytes[i]);
        const auto rawDataBytes_i = static_cast<uint8_t>(rawDataBytes[i]);
        TEST_ASSERT_EQ(bytes_i, rawDataBytes_i);
    }
}

static void read_raw_data(const fs::path& path, six::PixelType pixelType, std::span<const std::byte> expectedBytes)
{
    const auto expectedNumBytesPerPixel = pixelType == six::PixelType::RE32F_IM32F ? 8 : (pixelType == six::PixelType::AMP8I_PHS8I ? 2 : -1);

    const auto bytes = readFromNITF(path, pixelType);
    test_assert_eq(expectedBytes, bytes);

    six::sicd::NITFReadComplexXMLControl reader;
    reader.load(path);
    auto container = getContainer(reader);

    const auto pComplexData = getComplexData(*container, 0);
    auto& complexData = *pComplexData;
    test_assert(complexData, pixelType, expectedNumBytesPerPixel);

    const auto extent = getExtent(complexData);
    const types::RowCol<size_t> offset{ 0, 0 };

    if (pixelType == six::PixelType::RE32F_IM32F)
    {
        std::vector<std::complex<float>> rawData;
        six::sicd::Utilities::getRawData(reader.NITFReadControl(), complexData, offset, extent, rawData);
        test_assert_eq(bytes, rawData);
        test_assert_eq(expectedBytes, rawData);
    }
}

static void read_nitf(const fs::path& path, six::PixelType pixelType, const std::vector<std::complex<float>>& image)
{
    const auto expectedNumBytesPerPixel = pixelType == six::PixelType::RE32F_IM32F ? 8 : (pixelType == six::PixelType::AMP8I_PHS8I ? 2 : -1);
    const auto result = readSicd_(path, pixelType, expectedNumBytesPerPixel);
    TEST_ASSERT(result.widebandData == image);

    const auto bytes = six::sicd::testing::to_bytes(result);
    read_raw_data(path, pixelType, bytes);
}

static void buffer_list_save(const fs::path& outputName, const std::vector<std::complex<float>>& image,
    std::unique_ptr<six::sicd::ComplexData>&& pComplexData)
{
    six::XMLControlFactory::getInstance().addCreator<six::sicd::ComplexXMLControl>();
    six::NITFWriteControl writer(std::unique_ptr<six::Data>(std::move(pComplexData)));

    static const std::vector<std::string> schemaPaths;
    save(writer, image.data(), outputName.string(), schemaPaths); // API for Python; it uses six::BufferList
}

static void save(const fs::path& outputName, const std::vector<std::complex<float>>& image,
    std::unique_ptr<six::sicd::ComplexData>&& pComplexData)
{
    static const std::vector<fs::path> fs_schemaPaths;
    six::sicd::writeAsNITF(outputName, fs_schemaPaths, *pComplexData, image);
}

template<typename TSave>
static void test_create_sicd_from_mem_(const fs::path& outputName, six::PixelType pixelType, bool makeAmplitudeTable,
    TSave save)
{
    const types::RowCol<size_t> dims(2, 2);

    auto pComplexData = six::sicd::Utilities::createFakeComplexData(pixelType, makeAmplitudeTable, &dims);

    const auto expectedNumBytesPerPixel = pixelType == six::PixelType::RE32F_IM32F ? 8 : (pixelType == six::PixelType::AMP8I_PHS8I ? 2 : -1);
    test_assert(*pComplexData, pixelType, expectedNumBytesPerPixel);
    TEST_ASSERT_EQ(dims.row, pComplexData->getNumRows());
    TEST_ASSERT_EQ(dims.col, pComplexData->getNumCols());

    const auto image = make_complex_image(*pComplexData, dims);
    save(outputName, image, std::move(pComplexData));
    read_nitf(outputName, pixelType, image);
}
static void test_create_sicd_from_mem(const fs::path& outputName, six::PixelType pixelType, bool makeAmplitudeTable = false)
{
    test_create_sicd_from_mem_(outputName, pixelType, makeAmplitudeTable, save);
    test_create_sicd_from_mem_(outputName, pixelType, makeAmplitudeTable, buffer_list_save);
}

TEST_CASE(test_create_sicd_from_mem_32f)
{
    setNitfPluginPath();
    test_create_sicd_from_mem("test_create_sicd_from_mem_32f.sicd", six::PixelType::RE32F_IM32F);
}

TEST_MAIN((void)argc; (void)argv;
    TEST_CHECK(valid_six_50x50);
    TEST_CHECK(sicd_French_xml_raw);
    TEST_CHECK(sicd_French_xml);
    TEST_CHECK(test_readFromNITF_sicd_50x50);
    TEST_CHECK(test_read_sicd_50x50);
    TEST_CHECK(test_create_sicd_from_mem_32f);
    )
