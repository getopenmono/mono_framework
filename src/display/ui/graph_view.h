// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#ifndef graph_view_h
#define graph_view_h

#include "view.h"

namespace mono { namespace ui {

    
    /*!
     * @brief Data provider interface for the @ref GraphView class
     * 
     * This interface defines the *DataSource* for @ref GraphView objects. The 
     * graph view queries an associated data source object for the data to
     * display. All @ref GraphView objects must have a data source object that
     * hold the data to be displayed. The data source interface exposes an 
     * array-like scheme for providing data to the @ref GraphView.
     *
     * This interface forces you to expose your data sample as an array, where
     * all samples has an index. The index starts at index 0 and has variable
     * length.
     *
     * You must subclass this interface and override at least 3 virtual methods.
     * These 3 methods are:
     *
     * * @ref DataPoint(int) : Return individual data samples
     * * @ref BufferLength() : Return the full length of the data source
     * * @ref MaxSampleValueSpan() : Return the full range of the data sample values
     *
     * By providing the methods for retreiving data, getting the total length of
     * the data buffer and defining the valid value range of the data samples.
     *
     * If your subclass is representing a casual signal buffer, where samples
     * are continuesly written, you might override the method:
     *
     * * @ref NewestSampleIndex()
     *
     * This method should return the index to the newest sample in the buffer.
     * This enables the @ref GraphView to display a scrolling cursor, that moves
     * as the buffer data gets updates.
     *
     * **Note**: You are in charge of notifying the associated @ref GraphView's
     * when the data source content changes.
     *
     * @see GraphView
     */
    class IGraphViewDataSource
    {
    public:
        
        /*!
         * @brief Override this to return data point samples to the view
         * 
         * Provides a sample from the data source.
         * You must override this method to return samples at any given index.
         *
         * @param index The 0-indexed sample position, that should be returned
         * @return the sample value at the given index
         */
        virtual int DataPoint(int index) = 0;

        /*!
         * @brief Override this to return the data sample buffer length to the view
         * 
         * Returns the length / the total number of data samples in the data
         * source.
         * You must override this method to return the total number of data
         * sample in your data source implementation.
         *
         * @return the size / length of the data source
         */
        virtual int BufferLength() = 0;

        /*!
         * @brief The value span for the data samples.
         * üsed to map the buffer samples to the screen (view height)
         */
        virtual int MaxSampleValueSpan() = 0;
        
        /*!
         * @brief Return the position / index of the newest sample in the buffer
         *
         * Override this method to get a scrolling pointer on the `GraphView`,
         * a pointer that is drawn at this index position.
         *
         * @returns Position of newest sample
         */
        virtual int NewestSampleIndex() { return 0; };
    };

    
    /*!
     * @brief Visualizes a data series as a graph, based on a associated data source
     *
     * This class can visualize an array of samples on a graph. You provide the 
     * data by subclasseing the @ref IGraphViewDataSource to deliver to data to
     * display.
     *
     * This class only display the data, it does not hold or buffer it. In this
     * sense the GraphView contains no state.
     *
     * ### Example
     *
     * To demonstrate a simple example of using the GraphView we must also create
     * a data source. For an associated data source, let us wrap a simple C
     * array as a @ref IGraphViewDataSource subclass:
     *
     * @code
     *  
     *  // Subclass the IGraphViewDataSource interface
     *  class DataSource : public IGraphViewDataSource
     *  {
     *  private:
     *
     *      // Use an internal array as data store
     *      uint8_t data[100];
     *
     *  public:
     *
     *      // Override the method that provide data samples
     *      int DataPoint(int index) { return data[index]; }
     *
     *      // Override the method that return the total length of the data set
     *      int BufferLenght() { return 100; }
     *
     *      // Override the method that return the valid value range of the data
     *      // samples.
     *      int MaxSampleValueSpan() { return 256; }
     *  };
     *
     * @endcode
     *
     * The class `DataSource` is just an array with a length of 100. Note that
     * we only store 8-bit data samples (`uint_t`), therefore the valid data
     * range is 256. The GraphView expects the data values to be signed, meaning
     * the valid range is from -127 to +127.
     *
     * We have not provided any methods for putting data into the data source,
     * but we will skip that for this example.
     * 
     * Now, we can create a GraphView that displays data from the array:
     *
     * @code
     *
     *  // Crate the data source object
     *  DataSource ds;
     *
     *  // The view rectangle, where the graph is displayed
     *  mono::geo::Rect vRect(0,0,150,100);
     *
     *  // create the graph view, providing the display rect and data
     *  mono::ui::GraphView graph(vRect, ds);
     *
     *  //tell the graph view to be visible
     *  graph.show();
     *
     * @endcode
     *
     * ### Update Cursor
     * 
     * If you @ref IGraphViewDataSource subclass overrides the method: 
     * `NewestSampleIndex()`, the GraphView can show an update cursor. The cursor
     * is a vertical line drawn next to the latest or newest sample. The cursor
     * is hidden by default, but you can activate it by overiding the data source
     * method and calling @ref setCursorActive.
     *
     * Note that if you time span across the x-axis is less than 100 ms, the 
     * cursor might be annoying. I would recommend only using the cursor when
     * your graph updates slowly.
     *
     * ### Scaling and Zooming
     *
     * the graoh view will automatically scale down the data samples to be 
     * display inside its graph area. It displays the complete data set from the
     * data source, and does not support displaying ranges of the data set.
     *
     * If you wish to apply zooming (either on x or Y axis), you must do that by
     * scaling transforming the data in the data source. You can use an 
     * intermediate data source object, that scales the data samples, before
     * sending them to the view.
     *
     * @see IGraphViewDataSource
     */
    class GraphView : public View
    {
    protected:

        int bufferStride;
        float sampleMap;
        void privInit();
        bool useCursor;
        bool totalRefresh;
        int lastDataIndex;
        int lastDataIndexMapped;

        IGraphViewDataSource *source;

        void repaint();

        void updateStride();
        void updateMapping();

    public:
        
        /*!
         * @brief Construct a GraphView with no @ref viewRect and data source
         *
         * This contructor creates a GraphView object that needs a @ref viewRect
         * and @ref source to be set manually.
         *
         * You cannot display view objects that have a null-`viewRect`
         *
         * @see setDataSource
         * @see setRect
         */
        GraphView();

        /*!
         * @brief Construct a GraphView with a defined @ref viewRect
         *
         * This constructor takes a @ref viewRect, that defines the space where
         * graph is displayed.
         *
         * **No data will be displayed before you set a valid @ref source**
         *
         * @param rect The @ref viewRect where the graph is displayed
         * @see setDataSource
         */
        GraphView(geo::Rect rect);

        /*!
         * @brief Construct a GraphView with `viewRect` and a data source.
         *
         *
         *
         * @param
         * @param 
         */
        GraphView(geo::Rect rect, const IGraphViewDataSource &dSource);
        
        void setCursorActive(bool active);
        bool CursorActive() const;

        void setDataSource(const IGraphViewDataSource &dSrc);
        IGraphViewDataSource* DataSource() const;
    };

} }

#endif /* graph_view_h */
