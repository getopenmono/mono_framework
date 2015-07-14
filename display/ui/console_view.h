//
//  console_view.h
//  displaySimTest
//
//  Created by Kristoffer Andersen on 10/07/15.
//
//

#ifndef __displaySimTest__console_view__
#define __displaySimTest__console_view__

#include <view.h>
#include <display_painter.h>
#include <text_buffer.h>

namespace mono { namespace ui {
    
    template <uint16_t W, uint16_t H>
    class ConsoleView : public View {
    protected:
        
        /**
         * Text buffer that hold the visible text in the console.
         * When the console scrolls, the text in the buffer is overwritten.
         */
        TextBuffer<(W-4)/5, (H-4)/9> textBuffer;
        
        /**
         * Becomes true when the console text has reached the bottom line of its
         * view rectangle. And all text appending from now on, causes the console
         * to scroll.
         */
        bool scrolls;
        
        display::Color textColor;
        display::Color consoleColor;
        int lineSpacing;
        int softWrapIndent;
        int curLineIndex;
        
    public:
        
        /**
         * Construct a new ConsoleView, for viewing console output on the screen.
         *
         */
        ConsoleView() : View(),
            textColor(display::WhiteColor),
            consoleColor(display::BlackColor),
            lineSpacing(2),
            softWrapIndent(5)
        {
            View::painter.setTextSize(1);
            this->scrolls = false;
            this->curLineIndex = 0;
            this->size = geo::Size(W,H);
        }
        
        // Console methods
        
        /**
         * @brief Write a string to the console, and append a new line.
         * @param txt The string to write to the console
         */
        void WriteLine(const char *txt)
        {
            this->Write(txt, true);
            this->Write("\n");
        }
        
        void Write(const char *txt, bool noRepaint = false)
        {
            if (curLineIndex > 0)
                curLineIndex--; // overwrite the last string terminator, to concat strings
            
            char *sym = (char*) txt;
            while (curLineIndex < lineLength()) {
                
                textBuffer.insertToCurrentLine(curLineIndex, *sym);
                curLineIndex++;
                
                //soft wrap
                if (curLineIndex > lineLength())
                {
                    textBuffer.incrementLine();
                    curLineIndex = 3;
                    textBuffer.insertToCurrentLine(0,' ');
                    textBuffer.insertToCurrentLine(1,' ');
                    textBuffer.insertToCurrentLine(2,' ');
                }
                
                if (*sym == '\n')
                {
                    textBuffer.incrementLine();
                    curLineIndex = 0;
                }
                else if (*sym == '\0')
                {
                    break;
                }
                
                sym++;
            }
            
            if (!noRepaint)
                repaint();
        }
        
        void repaint()
        {
            View::painter.setForegroundColor(textColor);
            View::painter.setBackgroundColor(consoleColor);
            geo::Point cursor(position.X()+2,position.Y()+2);
            for (int l=textBuffer.OldestLinePosition(); l<textBuffer.LineCount(); l++) {
                //erase previous line
                View::painter.drawFillRect(cursor.X(), cursor.Y(), size.Width()-4, characterPixelHeight(), true);
                char *line = textBuffer.getLine(l);
                while (*line != '\0' && *line != '\n') {
                    View::painter.drawChar(cursor.X(), cursor.Y(), *line);
                    
                    cursor.appendX(characterPixelWidth());
                    line++;
                }
                cursor.appendY(characterPixelHeight()+lineSpacing);
                cursor.setX(2);
            }
            
            // have not scrolled yet, return
            if (textBuffer.OldestLinePosition() == 0)
                return;
                
                
            for (int l=0; l<=textBuffer.LinePosition(); l++) {
                //erase previous line
                View::painter.drawFillRect(cursor.X(), cursor.Y(), size.Width()-4, characterPixelHeight(), true);
                char *line = textBuffer.getLine(l);
                while (*line != '\0' && *line != '\n') {
                    View::painter.drawChar(cursor.X(), cursor.Y(), *line);
                    
                    cursor.appendX(characterPixelWidth());
                    line++;
                }
                cursor.appendY(characterPixelHeight()+lineSpacing);
                cursor.setX(2);
            }
        }
        
        // Auxilliary methods
        
        void setCursor(geo::Point pos)
        {
            this->textCursor = pos;
        }
        
        uint8_t characterPixelWidth()
        {
            return (View::painter.TextSize()*5);
        }
        
        uint8_t characterPixelHeight()
        {
            return (View::painter.TextSize()*7);
        }
        
        /**
         * <# description #>
         *
         * @brief Get the width of a line in characters
         * @returns Number of characters in one line
         */
        int lineLength()
        {
            return (W-4) / characterPixelWidth();
        }
        
        /**
         * <# description #>
         *
         * @brief Get the number lines in the console.
         * @returns Numeber of text lines on the console view.
         */
        int consoleLines()
        {
            return (H-4) / (characterPixelHeight()+lineSpacing);
        }
    };
 
    
} }


#endif /* defined(__displaySimTest__console_view__) */
