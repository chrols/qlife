attribute highp vec4 posAttribute;
attribute lowp vec4 colorAttribute;
varying lowp vec4 color;
uniform highp mat4 matrix;
uniform highp mat4 MVP;

void main() {
    color = colorAttribute;
    gl_Position = MVP * matrix * posAttribute;
}
