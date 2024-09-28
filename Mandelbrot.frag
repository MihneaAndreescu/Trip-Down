uniform sampler2D texture;
uniform float time;

float tr(float x)
{
    return (1 + sin(x)) * 0.5;
}

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    vec2 c = gl_TexCoord[0].xy;
    vec2 z = vec2(sin(time)*0.1, sin(time*0.4)*0.1);
    z = vec2(0, 0);
    int breakPoint = 256;
  
    for (int step = 0; step < 256; step++) {
        // z = z.x + z.y * i
        z = vec2(z.x * z.x - z.y * z.y, z.x * z.y + z.y * z.x) + c;
        //breakPoint = int(breakPoint < 10000) * breakPoint + int(breakPoint == 10000) * step * int(z.x * z.x + z.y * z.y > 16) + int(breakPoint == 10000) * breakPoint * int(z.x * z.x + z.y * z.y <= 16);

        if (dot(z, z) > 4) {
            breakPoint = step;
            break;
        }
    }
    //assert(0 <= breakPoint && breakPoint < (int)colors.size());
  
    float cop = breakPoint;

    
    pixel.r = cop / 256;
    pixel.g = cop / 256;
    pixel.b = cop / 256;
    pixel.r += 0.1 * (0.5 + 0.5 * sin(time*3+4));
    pixel.g += 0.1 * (0.5 + 0.5 * sin(time*5+6));
    pixel.b += 0.1 * (0.5 + 0.5 * sin(time*7+10));

    
    gl_FragColor = gl_Color * pixel;
    return;
    
    // lookup the pixel in the texture
   // vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // pixel.x = 1;
    // pixel.y = 0;
    // pixel.z = 0;
    // pixel.w = 1;

    //pixel.rbg = vec3(1, 0, 1); // swizzle operator


    if (breakPoint == 256)
    {   
        float rx = 0.1;
        float ry = 0.1;
        float gx = 0.1;
        float gy = 0.1;
        float bx = 0.1;
        float by = 0.1;
        pixel.r = tr(time + 0.5 + rx * gl_TexCoord[0].x + ry * gl_TexCoord[0].y);
        pixel.g = tr(time + 0.67 + gx * gl_TexCoord[0].x + gy * gl_TexCoord[0].y);
        pixel.b = tr(time + 0.39 + bx * gl_TexCoord[0].x + by * gl_TexCoord[0].y);
        //pixel.r = (1 + sin(gl_TexCoord[0].x * 66 + time)) * 0.5;
        //pixel.g = (1 + sin(gl_TexCoord[0].y * 66 + time + 0.111)) * 0.5;
       //pixel.b = (1 + sin(gl_TexCoord[0].x * 66 + time + 0.556)) * 0.5;
    }
    else
    {
        pixel.rgb = vec3(0, 0, 0);
    }

    gl_FragColor = gl_Color * pixel;


    //pixel.rgb = vec3(gl_TexCoord[0].x, gl_TexCoord[0].y, 0);

    // pixel.r = singl_TexCoord[0].x;
    // pixel.g = 0;
    // pixel.b = 0;
    // multiply it by the color
    //gl_FragColor = gl_Color * pixel;
}