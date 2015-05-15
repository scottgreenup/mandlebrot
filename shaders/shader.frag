#version 300 es
#
precision highp float;

uniform float time;
uniform sampler2D tex;

uniform float scale;
uniform vec2 center;

out vec4 color;

void main() {

    vec2 z, c;

    //vec2 center = vec2(0.7, 0.0);
    //float scale = 1.0;
    int iter = 240;

    c.x = (16.0 / 9.0) * (gl_FragCoord.x / 1920.0 - 0.5) * scale - center.x;
    c.y = (gl_FragCoord.y / 1080.0 - 0.5) * scale - center.y;


    int i;
    z = c;
    int m = 0;
    for(i=0; i<iter; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (z.y * z.x + z.x * z.y) + c.y;

        if((x * x + y * y) > 4.0) {
            m = i;
            color = vec4(float(i) / float(iter), 0.0, 0.0, 1.0);
            i = iter;
            break;
        }

        z.x = x;
        z.y = y;
    }
    

    color = texture(tex, vec2( (m == iter ? 0.0 : float(m) / float(iter)), 0));

}
