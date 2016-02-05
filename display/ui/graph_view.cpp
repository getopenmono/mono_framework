// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "graph_view.h"
#include <mbed_debug.h>
#include <us_ticker_api.h>

using namespace mono::ui;

GraphView::GraphView()
{
    privInit();
}

GraphView::GraphView(geo::Rect rect) : View(rect)
{
    privInit();
}

GraphView::GraphView(geo::Rect rect, const IGraphViewDataSource &dSrc) : View(rect)
{
    privInit();
    setDataSource(dSrc);
}

void GraphView::privInit()
{
    source = NULL;
    bufferStride = 1;
    useCursor = false;
    totalRefresh = true;
    lastDataIndex = 0;
    lastDataIndexMapped = 0;
}

// MARK: Getters and Setters

IGraphViewDataSource* GraphView::DataSource() const
{
    return source;
}

void GraphView::setDataSource(const mono::ui::IGraphViewDataSource &dSrc)
{
    if (source != NULL)
        scheduleRepaint();

    source = (IGraphViewDataSource*) &dSrc;
    totalRefresh = true;
    updateStride();
    updateMapping();
}

void GraphView::setCursorActive(bool active)
{
    if (useCursor != active)
        scheduleRepaint();
    
    useCursor = active;
}

bool GraphView::CursorActive() const
{
    return useCursor;
}

// MARK: Internal house keeping methods

void GraphView::updateStride()
{
    if (source == NULL || source->BufferLenght() <= 0)
    {
        bufferStride = 1;
        return;
    }

    if (viewRect.Width() <= 0)
    {
        bufferStride = 1;
        return;
    }

    if (source->BufferLenght() > viewRect.Width())
        bufferStride = source->BufferLenght() / viewRect.Width();
    else
        bufferStride = viewRect.Width() / source->BufferLenght();
}

void GraphView::updateMapping()
{
    if (source == NULL || source->MaxSampleValueSpan() <= 0)
    {
        sampleMap = 0.0;
        return;
    }

    if (viewRect.Height() <= 0)
    {
        sampleMap = 0.0;
        return;
    }

    sampleMap = viewRect.Height() / (source->MaxSampleValueSpan()*2.0);
}

void GraphView::repaint()
{
    if (source == NULL)
        return;

    geo::Rect updateView(viewRect);
    int newestIndex = source->NewestSampleIndex();

    if (!totalRefresh && lastDataIndex <= newestIndex)
    {
        updateView.appendY(1);
        updateView.setHeight(updateView.Height()-2);

        updateView.appendX(lastDataIndexMapped);
        int diff = (newestIndex - lastDataIndex) / bufferStride ;
        updateView.setWidth(diff < bufferStride ? bufferStride : diff );
    }
    else if (!totalRefresh)
    {
        updateView.appendY(1);
        updateView.setHeight(updateView.Height()-2);

        int diff = newestIndex / bufferStride ;
        updateView.setWidth(newestIndex < bufferStride ? bufferStride : diff);
    }

    //clear graph area
    painter.setBackgroundColor(StandardBackgroundColor);

    int start = us_ticker_read();
    painter.drawFillRect(updateView, true);
    int fillRect = us_ticker_read();

    if (totalRefresh)
    {
        painter.setForegroundColor(StandardBorderColor);
        painter.drawRect(viewRect);
        totalRefresh = false;
    }

    int outRect = us_ticker_read();

    // draw graph line
    bool viewSmallerThanBuffer = source->BufferLenght() > viewRect.Width();
    

    int newestIndexMapped = 0;

    int graphTime = 0;

    painter.setForegroundColor(StandardHighlightColor);
    for (int x=updateView.X(); x<updateView.X2(); x++)
    {
        if (viewSmallerThanBuffer)
        {
            int min, max;
            int xBuf = x*bufferStride;
            min = max = source->DataPoint(xBuf);

            if (newestIndex == xBuf)
                newestIndexMapped = x;

            for (int s=1; s<bufferStride; s++)
            {
                int sample = source->DataPoint(xBuf+s);
                if (sample < min)
                {
                    min = sample;
                }
                if (sample > max)
                    max = sample;
                
                if (newestIndex != 0 && newestIndex == xBuf+s)
                {
                    newestIndexMapped = x;
                }
            }
            
            int mapMax = sampleMap*max;
            int mapMin = sampleMap*min;
            
            if (mapMin == mapMax)
                mapMax++;

            int offset = updateView.Y()+updateView.Height()/2;

            int gBegin = us_ticker_read();
            painter.drawVLine(x, offset-mapMax, offset-mapMin);
            graphTime += us_ticker_read() - gBegin;
        }
        else
        {
            debug("buffers smaller than view length not yet supported!\n\r");
            return;
        }
    }
    int cursor = 0;
    //draw newest index position
    if (useCursor && newestIndex != 0 && newestIndexMapped != 0)
    {
        painter.setForegroundColor(StandardTextColor);

        int begin = us_ticker_read();
        if (bufferStride > 1)
            painter.drawVLine(viewRect.X()+newestIndexMapped, viewRect.Y(), viewRect.Y2());
        else
            painter.drawVLine(newestIndex, viewRect.Y(), viewRect.Y2());

        cursor = us_ticker_read() - begin;
    }

    lastDataIndex = newestIndex;
    lastDataIndexMapped = newestIndexMapped;

    int fill = fillRect - start;
    int out = outRect - fillRect;
    debug("Fill: %i, Outline: %i, graph: %i, cursor: %i, total: %i\n\r",fill,out,graphTime,cursor,fill+out+graphTime+cursor);
}