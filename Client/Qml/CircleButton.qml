import QtQuick 2.11

Rectangle{
    property double coordX
    property double coordY
    onCoordXChanged: {
        x=coordX-0.5*width
    }
    onCoordYChanged: {
        y=coordY-0.5*height
    }
    onWidthChanged: {
        x=coordX-0.5*width
    }
    onHeightChanged: {
        y=coordY-0.5*height
    }

    color: "white"
    radius: 0.5*width
}
