import QtQuick 2.9

Item {
    id: root

    property int size: 150
    property int lineWidth: 10
    property real value: 0
    property color primaryColor: "transparent"
    property color secondaryColor: "#a2238d"
    width: root.size
    height: root.size

    onValueChanged: { canvas.degree = value * 360; }

    Canvas {
        id: canvas
        property real degree: 0
        anchors.fill: parent
        antialiasing: true

        onDegreeChanged: { requestPaint(); }

        onPaint: {
            var ctx = getContext("2d");

            var x = root.width / 2 ;
            var y = root.height / 2;

            var radius = root.size / 2 - root.lineWidth
            var startAngle = (Math.PI / 180) * 270;
            var progressAngle = (Math.PI / 180) * (270 + degree);

            ctx.reset()

            ctx.lineCap = 'round';
            ctx.lineWidth = root.lineWidth;

            ctx.beginPath();
            ctx.arc(x, y, radius, startAngle, progressAngle);
            ctx.strokeStyle = root.primaryColor;
            ctx.stroke();
        }

        RotationAnimator {
               target: canvas;
               from: 0;
               to: 360;
               duration: 800
               running: true
               loops: Animation.Infinite
           }
    }
}
