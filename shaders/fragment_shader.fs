#version 330 core
out vec4 fragColor;

uniform float iTime;
uniform vec2  iResolution;
uniform vec3 color;

vec3 palette(float d){
	return mix(vec3(0.2,0.7,0.9),vec3(1.,0.,1.),d);
}

vec2 rotate(vec2 p,float a){
	float c = cos(a);
    float s = sin(a);
    return p*mat2(c,s,-s,c);
}

float map(vec3 p){
    for(int i = 0; i < 8; ++i){
        float t = iTime * 0.2;
        p.xz = rotate(p.xz, t);
        p.xy = rotate(p.xy, t * 1.89);
        p.xz = abs(p.xz);
        p.xz -= 0.5;
    }
    return dot(sign(p), p) / 5.0;
}

vec4 rm(vec3 ro, vec3 rd){
    float t = 0.0;
    vec3 col = vec3(0.0);
    float d;
    for(float i = 0.0; i < 64.0; i++){
        vec3 p = ro + rd * t;
        d = map(p) * 0.5;
        if(d < 0.02) break;
        if(d > 100.0) break;

        col += palette(length(p) * 0.1) / (400.0 * d);
        t += d;
    }
    return vec4(col, 1.0 / (d * 100.0));
}

void main1() {
    vec2 fragCoord = gl_FragCoord.xy;
    vec2 uv = (fragCoord - (iResolution.xy * 0.5)) / iResolution.x;

    vec3 ro = vec3(0.0, 0.0, -20.0);
    ro.xz = rotate(ro.xz, iTime);

    vec3 cf = normalize(-ro);
    vec3 cs = normalize(cross(cf, vec3(0.0, 1.0, 0.0)));
    vec3 cu = normalize(cross(cf, cs));
    vec3 uuv = ro + cf * 3.0 + uv.x * cs + uv.y * cu;
    vec3 rd = normalize(uuv - ro);

    vec4 col = rm(ro, rd);
    fragColor = col;
}


void main2(){
        // Grid size
    int tiles = 8; 
    
    //define the size of each grid item
    vec2 tileSize = iResolution.xy / float(tiles);
    // 
    vec2 tileCoord = mod(gl_FragCoord.xy, tileSize);
    
    // Center inside each tile
    vec2 centerTileCoord = tileCoord - tileSize * 0.5;
    
    // Normalize within each tile (preserve aspect)
    vec2 cord = centerTileCoord / iResolution.y;
    
    // Animation parameters
    float scale = 360.0;
    float t = mod(iTime, 60.0);
    float radius = length(cord);
    float wave = 0.5 + 0.5 * sin(radius * scale + t * 80 * 0.2);
    wave = smoothstep(0.5, 0.0, wave);
    
    // Animated RGB channels
    float r = wave * (0.5 + 0.5 * sin(iTime + 0.0));
    float g = wave * (0.5 + 0.5 * sin(iTime + 2.0));
    float b = wave * (0.5 + 0.5 * sin(iTime + 4.0));
    
    fragColor = vec4(r, g, b, 1.0);
    

}

void main3(){
    fragColor = vec4(color, 1.0f);
}


void main(){
    // main1();
    // main2();
    main3();
}