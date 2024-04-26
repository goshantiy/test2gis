import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: root
    width: 800
    height: 600
    visible: true
    FileDialog {
        id: fileDialog
        title: "Выберите файл"
        folder: shortcuts.home
        nameFilters: ["Text files (*.txt)"]
        onAccepted: {
            histogramModel.loadData(fileUrl)
        }
    }
    ColumnLayout {
        anchors.fill: parent
        Button {
            text: "Открыть файл"
            onClicked: {
                fileDialog.open()
                progressBar.visible = true
            }
            padding: 5
            Layout.alignment: Qt.AlignHCenter
        }
        Button {
            text: "Стоп"
            onClicked: {
                histogramModel.onStop()
            }
            padding: 5
            Layout.alignment: Qt.AlignHCenter
        }
        Button {
            text: "Пауза"
            onClicked: {
                histogramModel.onPause()
            }
            padding: 5
            Layout.alignment: Qt.AlignHCenter
        }
        ProgressBar {
            id: progressBar
            visible: false
            width: parent.width
            height: 30
            from: 0
            to: histogramModel.globalProgress
            value: histogramModel.progress
            Layout.alignment: Qt.AlignHCenter
        }
        Row {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Repeater {
                width: root.width
                model: histogramModel
                Layout.alignment: Qt.AlignHCenter
                delegate: ColumnLayout {
                    anchors.bottom: parent.bottom
                    Text {
                        text: model.count
                        Layout.alignment: Qt.AlignHCenter
                    }
                    Rectangle {
                        Layout.alignment: Qt.AlignBottom
                        color: "steelblue"
                        border.color: "black"
                        border.width: 1

                        width: root.width / 15

                        height: histogramModel.getBarHeight(index)
                    }
                    Text {
                        text: model.word
                        Layout.alignment: Qt.AlignHCenter
                    }
                }
            }
        }
    }
}
