// This software is part of OpenMono, see http://developer.openmono.com
// and is available under the MIT license, see LICENSE.txt

#include "rect.h"

using namespace mono::geo;

Rect::Rect(int x, int y, int width, int height) : geo::Point(x,y), geo::Size(width, height)
{
}

Rect::Rect(const geo::Point &p, const geo::Size &s) : geo::Point(p), geo::Size(s)
{
}

Rect::Rect()
{
    this->x = this->y = this->width = this->height = 0;
}

Rect::Rect(const Rect &r)
{
    this->x = r.x;
    this->y = r.y;
    this->width = r.width;
    this->height = r.height;
}

Rect& Rect::operator=(Rect r)
{
    this->x = r.x;
    this->y = r.y;
    this->width = r.width;
    this->height = r.height;
    return *this;
}

int Rect::X2() const
{
    return this->X() + this->Width();
}

int Rect::Y2() const
{
    return this->Y() + this->Height();
}

class Point Rect::Center() const
{
    return geo::Point( X() + Width()/2, Y() + Height()/2 );
}

Point Rect::UpperLeft() const
{
    return geo::Point(*this);
}

Point Rect::LowerRight() const
{
    return geo::Point(X2(), Y2());
}

Point Rect::UpperRight() const
{
    return geo::Point(X2(), Y());
}

Point Rect::LowerLeft() const
{
    return geo::Point(X(),Y2());
}

Point Rect::Point() const
{
    return geo::Point(*this);
}

Size& Rect::Size()
{
    return *this;
}

void Rect::setPoint(geo::Point p)
{
    this->x = p.X();
    this->y = p.Y();
}

void Rect::setSize(geo::Size s)
{
    this->width = s.Width();
    this->height = s.Height();
}

int Rect::Area()
{
    return Width() * Height();
}

bool Rect::contains(geo::Point &p) const
{
    return (p.X() >= this->x && p.X() <= this->X2() && p.Y() >= this->y && p.Y() <= this->Y2());
}

bool Rect::contains(const Rect &other, bool equals) const
{
    if (equals &&
        other.X() >= this->X() &&
        other.Y() >= this->Y() &&
        other.X2()<= this->X2()&&
        other.Y2()<= this->Y2())
    {
        return true;
    }
    else if (other.X() > this->X() &&
             other.Y() > this->Y() &&
             other.X2()< this->X2()&&
             other.Y2()< this->Y2())
        return true;
    else
        return false;
}

Rect Rect::crop(const mono::geo::Rect &other) const
{
    int x = this->X() > other.X() ? this->X() : other.X();
    int y = this->Y() > other.Y() ? this->Y() : other.Y();
    int x2 = this->X2() < other.X2() ? this->X2() : other.X2();
    int y2 = this->Y2() < other.Y2() ? this->Y2() : other.Y2();

    int w = x2 - x;
    int h = y2 - y;

    return Rect(x,y,w,h);
}

mono::String Rect::ToString() const
{
    return String::Format("Rect( %i, %i, %i, %i )",X(),Y(),Width(),Height());
}
