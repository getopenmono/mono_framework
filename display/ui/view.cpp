//
//  view.cpp
//  displaySimTest
//
//  Created by Kristoffer Andersen on 10/07/15.
//
//

#include "view.h"
#include <application_context.h>

using namespace mono::ui;

mono::display::DisplayPainter View::painter(mono::ApplicationContext::Instance->displayController);

View::View()
{
    
}

void View::setPosition(geo::Point pos)
{
    this->position = pos;
}

void View::setSize(geo::Size siz)
{
    this->size = siz;
}