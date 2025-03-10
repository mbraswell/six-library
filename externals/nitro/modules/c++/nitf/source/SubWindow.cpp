/* =========================================================================
 * This file is part of NITRO
 * =========================================================================
 *
 * (C) Copyright 2004 - 2014, MDA Information Systems LLC
 *
 * NITRO is free software; you can redistribute it and/or modify
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
 * License along with this program; if not, If not,
 * see <http://www.gnu.org/licenses/>.
 *
 */

#include "nitf/SubWindow.hpp"

#include <gsl/gsl.h>

#include "nitf/ImageSubheader.hpp"

using namespace nitf;

SubWindow::SubWindow(const SubWindow & x)
{
    *this = x;
}

void SubWindow::updateBandList()
{
    assert(bandList.has_value());
    setBandList(bandList->data());
    setNumBands(gsl::narrow<uint32_t>(bandList->size()));
}

SubWindow & SubWindow::operator=(const SubWindow & x)
{
    if (&x != this)
    {
        setNative(x.getNative());

        bandList = x.bandList;
        if (bandList.has_value())
        {
            updateBandList();
        }
    }
    return *this;
}

SubWindow::SubWindow(nitf_SubWindow * x)
{
    setNative(x);
    getNativeOrThrow();
}

SubWindow::SubWindow() noexcept(false) : SubWindow(nitf_SubWindow_construct(&error))
{
    setManaged(false);

    setStartCol(0);
    setStartRow(0);
}

SubWindow::SubWindow(uint32_t rows, uint32_t cols, uint32_t* bands, uint32_t numBands) : SubWindow()
{
    setNumRows(rows);
    setNumCols(cols);
    setBandList(bands);
    setNumBands(numBands);
}

static inline std::vector<uint32_t> iota(size_t count, uint32_t value = 0)
{
    std::vector<uint32_t> retval(count);
    std::iota(retval.begin(), retval.end(), value);
    return retval;
}
SubWindow::SubWindow(const ImageSubheader& subheader) :
    SubWindow(gsl::narrow<uint32_t>(subheader.numRows()), gsl::narrow<uint32_t>(subheader.numCols()))
{
    setBandList(iota(subheader.getBandCount()));
}

SubWindow::~SubWindow()
{
    if (isValid() && getNative()->downsampler)
    {
        nitf::DownSampler ds(getNativeOrThrow()->downsampler);
        //decrement the current DownSampler
        ds.decRef();
    }
}

uint32_t SubWindow::getStartRow() const
{
    return getNativeOrThrow()->startRow;
}

void SubWindow::setStartRow(uint32_t value)
{
    getNativeOrThrow()->startRow = value;
}

uint32_t SubWindow::getNumRows() const
{
    return getNativeOrThrow()->numRows;
}

void SubWindow::setNumRows(uint32_t value)
{
    getNativeOrThrow()->numRows = value;
}

uint32_t SubWindow::getStartCol() const
{
    return getNativeOrThrow()->startCol;
}

void SubWindow::setStartCol(uint32_t value)
{
    getNativeOrThrow()->startCol = value;
}

uint32_t SubWindow::getNumCols() const
{
    return getNativeOrThrow()->numCols;
}

void SubWindow::setNumCols(uint32_t value)
{
    getNativeOrThrow()->numCols = value;
}

uint32_t SubWindow::getBandList(int i)
{
    return getNativeOrThrow()->bandList[i];
}

void SubWindow::setBandList(uint32_t * value)
{
    getNativeOrThrow()->bandList = value;
}
void SubWindow::setBandList(std::vector<uint32_t>&& value)
{
    bandList = std::move(value);
    updateBandList();
}

uint32_t SubWindow::getNumBands() const
{
    return getNativeOrThrow()->numBands;
}

void SubWindow::setNumBands(uint32_t value)
{
    getNativeOrThrow()->numBands = value;
}

void SubWindow::setDownSampler(nitf::DownSampler* downSampler)
{
    if (getNativeOrThrow()->downsampler)
    {
        nitf::DownSampler ds(getNativeOrThrow()->downsampler);
        //decrement the current DownSampler
        ds.decRef();
    }

    if (downSampler != nullptr)
    {
        //increment the reference for this DownSampler
        getNativeOrThrow()->downsampler = downSampler->getNative();
        downSampler->incRef();
    }
    mDownSampler = downSampler;
}


nitf::DownSampler* SubWindow::getDownSampler() noexcept
{
    return mDownSampler;
}
const nitf::DownSampler* SubWindow::getDownSampler() const noexcept
{
    return mDownSampler;
}
