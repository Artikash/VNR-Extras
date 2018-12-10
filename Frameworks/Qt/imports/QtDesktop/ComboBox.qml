/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 1.1
import "custom" as Custom

/*
*
* ComboBox (inherits BasicButton)
*
* The ComboBox component is a combined button and popup list. The popup menu itself is platform
* native, and cannot by styled from QML code.
* Add menu items to the comboBox by either adding MenuItem children inside the popup, or
* assign it a ListModel (or both).
*
* The ComboBox contains the following API (in addition to the BasicButton API):
*
* ListModel model - this model will be used, in addition to MenuItem children, to
*   create items inside the popup menu
* bool popupOpen - setting this property to 'true' will open the popup.
* int selectedIndex - the index of the selected item in the popup menu.
* int hoveredIndex - the index of the highlighted item in the popup menu.
* string selectedText - the text of the selected menu item.
* string hoveredText - the text of the highlighted menu item.
*
* Example 1:
*
*    ListModel {
*        id: menuItems
*        ListElement { text: "Banana"; color: "Yellow" }
*        ListElement { text: "Apple"; color: "Green" }
*        ListElement { text: "Coconut"; color: "Brown" }
*    }
*    ComboBox {
*        model: menuItems
*        width: 200
*        onSelectedIndexChanged: console.debug(selectedText + ", " + menuItems.get(selectedIndex).color)
*    }
*
* Example 2:
*
*    ComboBox {
*        width: 200
*        MenuItem {
*            text: "Pineapple"
*            onTriggered: console.debug(text)
*
*        }
*        MenuItem {
*            text: "Grape"
*            onTriggered: console.debug(text)
*        }
*    }
*
*/

Custom.BasicButton {
    id: comboBox

    default property alias menuItems: popup.menuItems
    property alias model: popup.model
    property alias popupOpen: popup.visible

    property alias selectedIndex: popup.selectedIndex
    property alias hoveredIndex: popup.hoveredIndex
    property alias selectedText: popup.selectedText
    property alias hoveredText: popup.hoveredText
    property string styleHint

    background: StyleItem {
        anchors.fill: parent
        elementType: "combobox"
        sunken: comboBox.pressed
        raised: !sunken
        hover: comboBox.containsMouse
        enabled: comboBox.enabled
        text: comboBox.selectedText
        hasFocus: comboBox.focus
        contentHeight: 18
    }

//  ToDo: adjust margins so that selected popup label
//    centers directly above button label when
//    popup.centerOnSelectedText === true


    width: implicitWidth
    height: implicitHeight

    implicitWidth: Math.max(80, backgroundItem.implicitWidth)
    implicitHeight: backgroundItem.implicitHeight

    onWidthChanged: popup.setMinimumWidth(width)
    checkable: false

    onPressedChanged: if (pressed) popup.visible = true

    ContextMenu {
        id: popup
        property bool center: backgroundItem.styleHint("comboboxpopup")
        centerSelectedText: center
        y: center ? 0 : comboBox.height
    }

    // The key bindings below will only be in use when popup is
    // not visible. Otherwise, native popup key handling will take place:
    Keys.onSpacePressed: { comboBox.popupOpen = !comboBox.popupOpen }
    Keys.onUpPressed: { if (selectedIndex < model.count - 1) selectedIndex++ }
    Keys.onDownPressed: { if (selectedIndex > 0) selectedIndex-- }
}
