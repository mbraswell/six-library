/* =========================================================================
 * This file is part of xml.lite-c++ 
 * =========================================================================
 * 
 * (C) Copyright 2004 - 2014, MDA Information Systems LLC
 *
 * xml.lite-c++ is free software; you can redistribute it and/or modify
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

#ifndef __XML_LITE_ELEMENT_H__
#define __XML_LITE_ELEMENT_H__
#pragma once

#include <memory>
#include <string>
#include <new> // std::nothrow_t

#include <io/InputStream.h>
#include <io/OutputStream.h>
#include <str/Convert.h>
#include "xml/lite/XMLException.h"
#include "xml/lite/Attributes.h"
#include "xml/lite/QName.h"
#include "sys/Conf.h"
#include "sys/Optional.h"
#include "mem/SharedPtr.h"

/*!
 * \file  Element.h
 * \brief This contains the xml::lite data type for elements.
 *
 * This class is analogous to its DOM namesake.  It contains all of
 * the information that would be expected of it, including comment data.
 * The API, however, is much simpler and less developed than DOM.
 */

// xml::lite::Element
namespace xml
{
namespace lite
{
/*!
 * \class Element
 * \brief The class defining one element of an XML document
 *
 * This class stores all of the element information about an XML
 * document.
 */
class Element
{
    Element(const std::string& qname, const std::string& uri, std::nullptr_t) :
        mParent(nullptr), mName(uri, qname)
    {
    }

public:
    //! Default constructor
    Element() :
        mParent(nullptr)
    {
    }

    /*!
     * Constructor taking the namespace prefix and the local name 
     * \param qname The qname of the object
     * \param uri The uri of the object
     * \param characterData The character data (if any)
     */
    Element(const std::string& qname, const std::string& uri = "",
            const std::string& characterData = "") :
        Element(qname, uri, nullptr)
    {
        setCharacterData(characterData);
    }
    #ifndef SWIG  // SWIG doesn't like unique_ptr or StringEncoding
    Element(const std::string& qname, const std::string& uri,
            const std::string& characterData, StringEncoding encoding) :
        Element(qname, uri, nullptr)
    {
        setCharacterData(characterData, encoding);
    }
    Element(const std::string& qname, const std::string& uri,
            const sys::U8string& characterData) :
        Element(qname, uri, nullptr)
    {
        setCharacterData(characterData);
    }

    // StringEncoding is assumed based on the platform: Windows-1252 or UTF-8.
    static std::unique_ptr<Element> create(const std::string& qname, const std::string& uri = "", const std::string& characterData = "");
    static std::unique_ptr<Element> create(const std::string& qname, const xml::lite::Uri& uri, const std::string& characterData = "");
    static std::unique_ptr<Element> create(const xml::lite::QName&, const std::string& characterData = "");
    static std::unique_ptr<Element> create(const xml::lite::QName&, const sys::U8string&);
    // Encoding of "characterData" is always UTF-8
    static std::unique_ptr<Element> createU8(const xml::lite::QName&, const std::string& characterData = "");
    #endif // SWIG

    //! Destructor
    virtual ~Element()
    {
        destroyChildren();
    }

    //! Destroys any child elements.
    void destroyChildren();

    // use clone() to duplicate an Element
#if !(defined(SWIG) || defined(SWIGPYTHON) || defined(HAVE_PYTHON_H))  // SWIG needs these
//private: // encoded as part of the C++ name mangling by some compilers
#endif
    Element(const Element&);
    Element& operator=(const Element&);
#if !(defined(SWIG) || defined(SWIGPYTHON) || defined(HAVE_PYTHON_H))
public:
#endif

    Element(Element&&) = default;
    Element& operator=(Element&&) = default;

    /*!
     *  Clone function performs deep copy
     *  of element
     *  \param element  Takes an element
     */
    void clone(const Element& element);

    /*!
     *  Get the attributes in a non-const way
     *  \return attributes
     */
    Attributes& getAttributes()
    {
        return mAttributes;
    }

    std::string& attribute(const std::string& s)
    {
        return mAttributes[s];
    }

    /*!
     *  Get the attributes in a const way
     *  \return attributes
     */
    const Attributes& getAttributes() const
    {
        return mAttributes;
    }

    /*!
     *  Set the attributes
     *  \param attributes The attributes to set
     */
    void setAttributes(const Attributes& attributes)
    {
        mAttributes.clear();
        for (int i = 0; i < attributes.getLength(); i++)
        {
            mAttributes.add(attributes.getNode(i));
        }
    }

    /*!
     *  Get the elements by tag name
     *  \param qname the QName
     *  \param elements the elements that match the QName
     */
    void getElementsByTagNameNS(const std::string& qname,
                                std::vector<Element*>& elements,
                                bool recurse = false) const;
    /*!
     *  \param std::nothrow -- will still throw if MULTIPLE elements are found, returns NULL if none
     */
    Element* getElementByTagNameNS(std::nothrow_t, const std::string& qname, bool recurse = false) const;
    Element& getElementByTagNameNS(const std::string& qname, bool recurse = false) const;

    /*!
     *  Utility for people that dont like to pass by reference
     *
     */
    std::vector<Element*> getElementsByTagNameNS(const std::string& qname, bool recurse = false) const
    {
        std::vector<Element*> v;
        getElementsByTagNameNS(qname, v, recurse);
        return v;
    }

    /*!
     *  Sometimes we dont care about the qname or the uri -- just 
     *  the local name is good enough.  For those times, use this function
     *  \param localName The local name
     *  \param elements The elements
     */
    void getElementsByTagName(const std::string& localName,
                              std::vector<Element*>& elements,
                              bool recurse = false) const;

    /*!
     *  Utility for people that dont like to pass by reference
     */
    std::vector<Element*> getElementsByTagName(const std::string& localName,
                                               bool recurse = false) const
    {
        std::vector<Element*> v;
        getElementsByTagName(localName, v, recurse);
        return v;
    }

    /*!
     *  \param std::nothrow -- will still throw if MULTIPLE elements are found, returns NULL if none
     */
    Element* getElementByTagName(std::nothrow_t, const std::string& localName, bool recurse = false) const;
    Element& getElementByTagName(const std::string& localName, bool recurse = false) const;

    /*!
     *  Get the elements by tag name
     *  \param uri the URI
     *  \param localName the local name
     *  \param elements the elements that match the QName
     */
    void getElementsByTagName(const xml::lite::QName& name, std::vector<Element*>& elements, bool recurse = false) const;
    void getElementsByTagName(const std::string& uri, const std::string& localName, std::vector<Element*>& elements, bool recurse = false) const
    {
        getElementsByTagName(QName(uri, localName), elements, recurse);
    }
    
    /*!
     *  \param std::nothrow -- will still throw if MULTIPLE elements are found,
     * returns NULL if none
     */
    Element* getElementByTagName(std::nothrow_t, const xml::lite::QName&, bool recurse = false) const;
    Element* getElementByTagName(std::nothrow_t t, const std::string& uri, const std::string& localName, bool recurse = false) const 
    {
        return getElementByTagName(t, QName(uri, localName), recurse);
    }
    Element& getElementByTagName(const xml::lite::QName&, bool recurse = false) const;
    Element& getElementByTagName(const std::string& uri, const std::string& localName, bool recurse = false) const 
    {
        return getElementByTagName(QName(uri, localName), recurse);
    }

    /*!
     *  Utility for people that dont like to pass by reference
     */
    std::vector<Element*> getElementsByTagName(const xml::lite::QName& name, bool recurse = false) const
    {
        std::vector<Element*> v;
        getElementsByTagName(name, v, recurse);
        return v;
    }
    std::vector<Element*> getElementsByTagName(const std::string& uri, const std::string& localName, bool recurse = false) const
    {
        return getElementsByTagName(QName(uri, localName), recurse);
    }

    /*!
     *  1)  Find this child's attribute and change it
     *  2)  Recursively descend over children and fix all
     *  namespaces below using fixNodeNamespace()
     */
    void setNamespacePrefix(std::string prefix, const xml::lite::Uri&);
    void setNamespacePrefix(std::string prefix, std::string uri)
    {
        setNamespacePrefix(prefix, Uri(uri));
    }

    void setNamespaceURI(std::string prefix, const xml::lite::Uri&);
    void setNamespaceURI(std::string prefix, std::string uri)
    {
        setNamespaceURI(prefix, Uri(uri));
    }

    /*!
     *  Prints the element to the specified OutputStream
     *  \param stream the OutputStream to write to
     *  \param formatter  The formatter
     *  \todo Add format capability
     */
    void print(io::OutputStream& stream) const;

    // This is another slightly goofy routine to maintain backwards compatibility.
    // XML documents must be properly (UTF-8, UTF-16 or UTF-32).  The legacy
    // print() routine (above) can write documents with a Windows-1252 encoding
    // as the string is just copied to the output.
    //
    // The only valid setting for StringEncoding is Utf8; but defaulting that
    // could change behavior on Windows.
    void prettyPrint(io::OutputStream& stream,
                     const std::string& formatter = "    ") const;
    #ifndef SWIG  // SWIG doesn't like unique_ptr or StringEncoding
    void print(io::OutputStream& stream, StringEncoding /*=Utf8*/) const;
    void prettyPrint(io::OutputStream& stream, StringEncoding /*=Utf8*/,
                     const std::string& formatter = "    ") const;
    #endif // SWIG

    /*!
     *  Determines if a child element exists
     *  \param localName the local name to search for
     *  \return true if it exists, false if not
     */
    bool hasElement(const std::string& localName) const;

    /*!
     *  Determines if a child element exists
     *  \param uri the URI to search for
     *  \param localName the local name to search for
     *  \return true if it exists, false if not
     */
    bool hasElement(const xml::lite::QName&) const;
    bool hasElement(const std::string& uri, const std::string& localName) const
    {
        return hasElement(QName(uri, localName));
    }

    /*!
     *  Returns the character data of this element.
     *  \return the charater data
     */
    std::string getCharacterData() const
    {
        return mCharacterData;
    }
    #ifndef SWIG  // SWIG doesn't like unique_ptr or StringEncoding
    const sys::Optional<StringEncoding>& getEncoding() const
    {
        return mEncoding;
    }
   const sys::Optional<StringEncoding>& getCharacterData(std::string& result) const
    {
        result = getCharacterData();
        return getEncoding();
    }
    void getCharacterData(sys::U8string& result) const;
    #endif // SWIG

    /*!
     *  Sets the character data for this element.
     *  \param characters The data to add to this element
     */
    void setCharacterData(const std::string& characters);
    #ifndef SWIG  // SWIG doesn't like unique_ptr or StringEncoding
    void setCharacterData_(const std::string& characters, const StringEncoding*);
    void setCharacterData(const std::string& characters, StringEncoding);
    void setCharacterData(const sys::U8string& characters);
    #endif // SWIG

    /*!
     *  Sets the local name for this element.
     *  \param localName the data to add to this element
     */
    void setLocalName(const std::string& localName)
    {
        mName.setName(localName);
    }

    /*!
     *  Returns the local name of this element.
     *  \return the local name
     */
    std::string getLocalName() const
    {
        return mName.getName();
    }

    /*!
     *  Sets the QName for this element.
     *  \param qname the data to add to this element
     */
    void setQName(const std::string& qname)
    {
        mName.setQName(qname);
    }
    void setQName(const xml::lite::QName& qname)
    {
        mName = qname;
    }

    /*!
     *  Returns the QName of this element.
     *  \return the QName
     */
    std::string getQName() const
    {
        QName result;
        getQName(result);
        return result.toString();
    }
    void getQName(xml::lite::QName& result) const
    {
        result = mName;
    }

    /*!
     *  Sets the URI for this element.
     *  \param uri the data to add to this element
     */
    void setUri(const xml::lite::Uri& uri)
    {
        mName.setAssociatedUri(uri);
    }
    void setUri(const std::string& uri)
    {
        setUri(Uri(uri));
    }

    /*!
     *  Returns the URI of this element.
     *  \return the URI
     */
    std::string getUri() const
    {
        Uri result;
        getUri(result);
        return result.value;
    }
    void getUri(xml::lite::Uri& result) const
    {
        mName.getAssociatedUri(result);
    }

    /*!
     *  Adds a child element to this element
     *  \param node the child element to add
     */
    virtual void addChild(Element * node);

    /*!
     *  Adds a child element to this element
     *  \param node the child element to add
     */
    virtual Element& addChild(std::unique_ptr<Element>&& node);
    #if CODA_OSS_autoptr_is_std  // std::auto_ptr removed in C++17
    virtual Element& addChild(mem::auto_ptr<Element> node);
    #endif

    /*!
     *  Returns all of the children of this element
     *  \return the children of this element
     */
    std::vector<Element*>& getChildren()
    {
        return mChildren;
    }

    /*!
     *  Returns all of the children of this element
     *  \return the children of this element
     */
    const std::vector<Element*>& getChildren() const
    {
        return mChildren;
    }

    /*!
    * Removes all the children WITHOUT destroying them; see destroyChildren().
    */
    void clearChildren()
    {
        mChildren.clear();
    }

    Element* getParent() const
    {
        return mParent;
    }

    void setParent(Element* parent)
    {
        mParent = parent;
    }

protected:

    void changePrefix(Element* element,
                      const std::string& prefix,
                      const std::string& uri);

    void changeURI(Element* element,
                   const std::string& prefix,
                   const std::string& uri);

    void depthPrint(io::OutputStream& stream, int depth,
                    const std::string& formatter) const;
    void depthPrint(io::OutputStream& stream, StringEncoding, int depth,
                    const std::string& formatter) const;

    Element* mParent;
    //! The children of this element
    std::vector<Element*> mChildren;
    xml::lite::QName mName;
    //! The attributes for this element
    xml::lite::Attributes mAttributes;
    //! The character data ...
    std::string mCharacterData;

    private:
        // ... and how that data is encoded
        sys::Optional<StringEncoding> mEncoding;
        void depthPrint(io::OutputStream& stream, bool utf8, int depth,
                const std::string& formatter) const;
};

extern Element& add(const xml::lite::QName&, const std::string& value, Element& parent);

#ifndef SWIG
// The (old) version of SWIG we're using doesn't like certain C++11 features.

/*!
 *  Returns the character data of this element converted to the specified type.
 *  \param value the charater data as T
 *  \return whether or not there was a value of type T
 */
template <typename ToType>
inline auto castValue(const Element& element, ToType toType)  // getValue() conflicts with below
   -> decltype(toType(std::string()))
{
    const auto characterData = element.getCharacterData();
    if (characterData.empty())
    {
        throw except::BadCastException(Ctxt("call getCharacterData() to get an empty string"));
    }
    return toType(characterData);
}
template <typename T>
inline T getValue(const Element& element)
{
    return castValue(element, details::toType<T>);
}

template <typename T, typename ToType>
inline bool castValue(const Element& element, T& value, ToType toType)
{
    try
    {
        value = castValue(element, toType);
    }
    catch (const except::BadCastException&)
    {
        return false;
    }
    return true;
}
template <typename T>
inline bool getValue(const Element& element, T& value)
{
    return castValue(element, value, details::toType<T>);
}

/*!
 *  Sets the character data for this element by calling str::toString() on the value.
 *  \param value The data to add to this element
 */
template <typename T, typename ToString>
inline void setValue(Element& element, const T& value, ToString toString)
{
    element.setCharacterData(toString(value));
}
template <typename T>
inline void setValue(Element& element, const T& value)
{
    setValue(element, value, details::toString<T>);
}

template <typename T, typename ToString>
inline Element& addNewElement(const xml::lite::QName& name, const T& value, Element& parent,
    ToString toString)
{
    return xml::lite::add(name, toString(value), parent);
}
template<typename T>
inline Element& addNewElement(const xml::lite::QName& name, const T& value, Element& parent)
{
    return addNewElement(name, value, parent, details::toString<T>);
}

template <typename T, typename ToString>
inline Element& addNewElement(const xml::lite::QName& name, const sys::Optional<T>& v, Element& parent,
    ToString toString)
{
    return addNewElement(name, v.value(), parent, toString);
}
template<typename T>
inline Element& addNewElement(const xml::lite::QName& name, const sys::Optional<T>& v, Element& parent)
{
    return addNewElement(name, v.value(), parent);
}
template <typename T, typename ToString>
inline Element* addNewOptionalElement(const xml::lite::QName& name, const sys::Optional<T>& v, Element& parent,
        ToString toString)
{
    return v.has_value() ? &addNewElement(name, v, parent, toString) : nullptr;
}
template<typename T>
inline Element* addNewOptionalElement(const xml::lite::QName& name, const sys::Optional<T>& v, Element& parent)
{
    return v.has_value() ? &addNewElement(name, v, parent) : nullptr;
}

#endif // SWIG

}
}

#endif
