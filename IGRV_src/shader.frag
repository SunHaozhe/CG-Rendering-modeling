// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Shaders
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

// Add here all the values you need to describe the lights or the materials.
// At first used const values, eventually stored in structures.
// Then, use uniform variables and set them from your CPU program using
// the GLProgram methods.

uniform vec3 lightPos0;
uniform vec3 lightCol0;
uniform vec3 lightPos1;
uniform vec3 lightCol1;
uniform vec3 lightPos2;
uniform vec3 lightCol2;


const vec3 matAlbedo = vec3 (0.6, 0.6, 0.6);
const float ks = float(1.0);
const float s = float(1.0);

varying vec4 P; // Interpolated fragment-wise position
varying vec3 N; // Interpolated fragment-wise normal
varying vec4 C; // Interpolated fragment-wise color

void main (void) {

    vec3 x = vec3 (gl_ModelViewMatrix * P);       //x
    vec3 n = normalize (gl_NormalMatrix * N);     //n
    vec3 wi0 = normalize (lightPos0 - x);            //wi
    vec3 wi1 = normalize (lightPos1 - x);            //wi
    vec3 wi2 = normalize (lightPos2 - x);            //wi
    vec3 wo = normalize (-x);                      //wo
    vec3 wh0 = normalize (wi0 + wo);                 //wh
    vec3 wh1 = normalize (wi1 + wo);                 //wh
    vec3 wh2 = normalize (wi2 + wo);                 //wh

    float attenuation0 = float(1.0);
    float attenuation1 = float(1.0);
    float attenuation2 = float(1.0);
    vec3 L0 = lightCol0;
    vec3 L1 = lightCol1;
    vec3 L2 = lightCol2;
    float fd = float(1.0);
    float fs0 = ks * pow(dot(n, wh0), s);
    float fs1 = ks * pow(dot(n, wh1), s);
    float fs2 = ks * pow(dot(n, wh2), s);
    float f0 = fd + fs0;
    float f1 = fd + fs1;
    float f2 = fd + fs2;

    vec3 color = vec3(0.0, 0.0, 0.0);
    color += L0* f0 * max(dot(n, wi0), 0.0) * attenuation0;
    color += L1* f1 * max(dot(n, wi1), 0.0) * attenuation1;
    color += L2* f2 * max(dot(n, wi2), 0.0) * attenuation2;

    gl_FragColor = vec4(color, 1.0);
}
