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
import "custom" as Components

Item {
    id: tabWidget
    width: 100
    height: 100
    property int current: 0
    property int count: stack.children.length
    property bool frame: true
    property bool tabsVisible: true
    property string position: "North"
    default property alias tabs : stack.children
    property Item tabBar: tabbarItem

    onCurrentChanged: __setOpacities()
    Component.onCompleted: __setOpacities()

    property int __baseOverlap : frameitem.pixelMetric("tabbaseoverlap")// add paintmargins;
    function __setOpacities() {
        for (var i = 0; i < stack.children.length; ++i) {
            stack.children[i].visible = (i == current ? true : false)
        }
    }

    Component {
        id: tabcomp
        Tab {}
    }

    function addTab(component, title) {
        var tab = tabcomp.createObject(this);
        component.createObject(tab)
        tab.parent = stack
        tab.title = title
        __setOpacities()
        return tab
    }

    function removeTab(id) {
        var tab = tabs[id]
        tab.destroy()
        if (current > 0)
            current-=1
    }

    StyleItem {
        id: frameitem
        z: style == "oxygen" ? 1 : 0
        elementType: "tabframe"
        info: position
        value: tabbarItem && tabsVisible && tabbarItem.tab(current) ? tabbarItem.tab(current).x : 0
        minimum: tabbarItem && tabsVisible && tabbarItem.tab(current) ? tabbarItem.tab(current).width : 0
        maximum: tabbarItem && tabsVisible ? tabbarItem.tabWidth : width
        anchors.fill: parent

        property int frameWidth: pixelMetric("defaultframewidth")

        Item {
            id: stack
            anchors.fill: parent
            anchors.margins: (frame ? frameitem.frameWidth : 0)
            anchors.topMargin: anchors.margins + (frameitem.style =="mac" ? 6 : 0)
            anchors.bottomMargin: anchors.margins + (frameitem.style =="mac" ? 6 : 0)
        }

        anchors.topMargin: tabbarItem && tabsVisible && position == "North" ? Math.max(0, tabbarItem.height - __baseOverlap) : 0

        states: [
            State {
                name: "South"
                when: position == "South" && tabbarItem!= undefined
                PropertyChanges {
                    target: frameitem
                    anchors.topMargin: 0
                    anchors.bottomMargin: tabbarItem ? tabbarItem.height - __baseOverlap: 0
                }
                PropertyChanges {
                    target: tabbarItem
                    anchors.topMargin: -__baseOverlap
                }
                AnchorChanges {
                    target: tabbarItem
                    anchors.top: frameitem.bottom
                    anchors.bottom: undefined
                }
            }
        ]
    }
    TabBar {
        id: tabbarItem
        tabFrame: tabWidget
        anchors.top: tabWidget.top
        anchors.left: tabWidget.left
        anchors.right: tabWidget.right
    }
}
