//
//  brdf.h
//  PBRLib
//
//  Created by Асиф Мамедов on 15/08/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef brdf_h
#define brdf_h

#define PI 		3.14159265359
#define M_1_PI 	0.31830988618

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define D_BECKAMNN_ID 			0
#define D_GGX_ID 				1
#define D_GGX_ANISOTROPY_ID		2

float D_Beckmann(mediump float n_dot_m, mediump float alpha)
{
    float n_dot_m_2 = n_dot_m * n_dot_m;
    float alpha_2 	= alpha * alpha;
    
    return exp(((n_dot_m_2 - 1.0) / n_dot_m_2) / alpha_2) / (PI * alpha_2 * n_dot_m_2 * n_dot_m_2);
}

float D_GGX(mediump float n_dot_m, mediump float alpha)
{
	float alpha_pow_2 	= alpha * alpha;
	float denom 		= n_dot_m * n_dot_m * (alpha_pow_2 - 1.0f) + 1.0f; 

	denom *= denom * PI;

	return alpha_pow_2 / max(denom, 0.00001);
}

float D_GGX_Anisotropy(
	mediump float n_dot_h,
	mediump float tang_dot_h,
	mediump float btang_dot_h,
	mediump float alpha,
	mediump float anisotropy
)
{
#if 1
	float alpha_t = max(alpha * (1.0 + anisotropy), 0.001);
	float alpha_b = max(alpha * (1.0 - anisotropy), 0.001);
#elif 0
	float alpha_t = alpha;
	float alpha_b = mix(0.0, alpha, 1.0f - anisotropy);
#else 
	float alpha_t = max(alpha * (1.0 + anisotropy), 0.001);
	float alpha_b = max(alpha * (1.0 - anisotropy), 0.001);
#endif 

    float a = alpha_b * alpha_t;

    vec3 	v 	= vec3(alpha_b * tang_dot_h, alpha_t * btang_dot_h, a * n_dot_h);
    float 	v2 	= dot(v, v);
    float	w 	= a / v2;

    return a * w * w * (1.0 / PI);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define G1_GGX              0
#define G1_BECKMANN         1
#define G1_SCHLICK_BECKMANN 2
#define G1_SCHLICK_GGX      3

#define G_SMITH_USE G1_GGX

#define G_BECKAMNN_ID 				0
#define G_GGX_ID 					1
#define G_SCHILCK_BECKAMNN_ID 		2
#define G_SCHILCK_GGX_ID 			3
#define G_IMPLICIT_ID				4
#define G_NEUMANN_ID				5
#define G_COOK_TARRANCE_ID			6
#define G_KELEMEN_ID				7
#define G_SMITH_ID					8

float G_GGX(mediump float n_dot_m, mediump float alpha)
{
	float alpha_pow_2	= alpha * alpha;

	float numerator 	= 2.0f * n_dot_m;
	float denominator 	= n_dot_m + sqrt(alpha_pow_2 + (1.0f - alpha_pow_2) * n_dot_m * n_dot_m);

	return numerator / denominator;
}

float G_Beckmann(mediump float n_dot_m, mediump float alpha)
{
	float c = n_dot_m / (alpha * sqrt(1.0f - n_dot_m * n_dot_m));

	if (c >= 1.6) {
		return 1.0f;
	} 

	float c_pow_2 = c * c;

	float numerator		= 3.535 * c + 2.181 * c_pow_2;
	float denominator	= 1.0f + 2.276 * c + 2.577 * c_pow_2;

	return numerator / denominator;
}

float G_SchlickBeckmann(mediump float n_dot_m, mediump float alpha)
{
	float k = alpha * sqrt(2.0f / PI);

	return n_dot_m / (n_dot_m * (1.0f - k) + k); 
}

float G_SchlickGGX(mediump float n_dot_m, mediump float alpha)
{
	float k = alpha /2.0f;

	return n_dot_m / (n_dot_m * (1.0f - k) + k); 
}

float G_Implicit(mediump float n_dot_l, mediump float n_dot_v)
{
	return n_dot_l * n_dot_v;
}

float G_Neumann(mediump float n_dot_l, mediump float n_dot_v)
{
	return (n_dot_l * n_dot_v) / max(n_dot_l, n_dot_v);
}

float G_CookTorrance(
	mediump float n_dot_h,
	mediump float v_dot_h,
	mediump float n_dot_v,
	mediump float n_dot_l
)
{
	float n_dot_h_2 = 2.0f * n_dot_h;

	return min(1.0f, min((n_dot_h_2 * n_dot_v) / v_dot_h, (n_dot_h_2 * n_dot_l) / v_dot_h));
}

float G_Kelemen(mediump float v_dot_h, mediump float n_dot_v, mediump float n_dot_l)
{
	return (n_dot_l * n_dot_v) / (v_dot_h * v_dot_h);
}

float G_Smith(float n_dot_l, float n_dot_v, float alpha)
{
#if G_SMITH_USE == G1_GGX
	return G_GGX(n_dot_l, alpha) * G_GGX(n_dot_v, alpha);
#elif G_SMITH_USE == G1_BECKMANN
	return G_Beckmann(n_dot_l, alpha) * G_Beckmann(n_dot_v, alpha);
#elif G_SMITH_USE == G1_SCHLICK_BECKMANN
	return G_SchlickBeckmann(n_dot_l, alpha) * G_SchlickBeckmann(n_dot_v, alpha);
#else 
	return G_SchlickGGX(n_dot_l, alpha) * G_SchlickGGX(n_dot_v, alpha);
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define F_NONE_ID 			0
#define F_SCHLICK_ID		1
#define F_COOK_TORRANCE_ID 	2

vec3 F_None(lowp const vec3 f0)
{
	return f0;
}

vec3 F_Schlick(lowp const vec3 f0, mediump float v_dot_h)
{
	return f0 + (1.0 - f0) * pow(1.0 - v_dot_h, 5.0);
}

vec3 F_CookTorrance(lowp const vec3 f0, mediump float v_dot_h)
{
	vec3 sqrt_f0 = sqrt(f0);

	vec3 n = (1.0f + sqrt_f0) / (1.0f - sqrt_f0);
	vec3 g = sqrt(n * n + v_dot_h * v_dot_h - 1.0f);

	vec3 a = g - v_dot_h;
	vec3 b = g + v_dot_h;

	vec3 c = a / b;
	vec3 d = (b * v_dot_h - 1) / (a * v_dot_h + 1.0f);

	return 0.5 * c * c * (1.0f + d * d);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float getSpotAngleAttenuation(in mediump const vec3 l, in mediump const vec3 light_dir, float inner_angle, float outer_angle) 
{
	float l_dot_ld 		= max(dot(normalize(-light_dir), l), 0.000001f);
	float cos_outter	= cos(outer_angle);
	
	return (l_dot_ld - cos_outter) / (cos(inner_angle) - cos_outter);
}

#endif /* brdf_h */
