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

Item {
    id: groupbox

    implicitWidth: adjustToContentSize ? Math.max(200, contentWidth + loader.leftMargin + loader.rightMargin) : 100
    implicitHeight: adjustToContentSize ? contentHeight + loader.topMargin + loader.bottomMargin : 100

    default property alias data: content.data

    property string title
    property bool checkable: false
    property int contentWidth: content.childrenRect.width
    property int contentHeight: content.childrenRect.height
    property double contentOpacity: 1

    property Component background: null
    property Item backgroundItem: loader.item

    property Item checkbox: check
    property alias checked: check.checked
    property bool adjustToContentSize: false // Resizes groupbox to fit contents.
                                             // Note when using this, you cannot anchor children
    Loader {
        id: loader
        anchors.fill: parent
        property int topMargin: title.length > 0 || checkable ? 22 : 4
        property int bottomMargin: 4
        property int leftMargin: 4
        property int rightMargin: 4
        property alias styledItem: groupbox
        sourceComponent: background
    }
    CheckBox {
        id: check
        checked: true
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: loader.topMargin
    }
    Item {
        id:content
        z: 1
        focus: true
        opacity: contentOpacity
        anchors.topMargin: loader.topMargin
        anchors.leftMargin: 8
        anchors.rightMargin: 8
        anchors.bottomMargin: 8
        anchors.fill: parent
        enabled: (!checkable || checkbox.checked)
    }
}
