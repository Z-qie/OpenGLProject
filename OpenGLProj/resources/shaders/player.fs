#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct Material {
    vec3 ambient; 
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
// wired enough that the vec3 doesnt work in UBO alignment!!!
// If you want the effect of a vec3 + a float, then you should pack it manually in vec4
struct DirLight {
    vec3 direction;  float dummy0;
    vec3 ambient;    float dummy1;
    vec3 diffuse;    float dummy2;
    vec3 specular;   float dummy3;
};

struct PointLight {
    vec3 position;  float dummy0;
    
    float constant;
    float linear;
    float quadratic;    float dummy1;

    vec3 ambient;   	float dummy2;
    vec3 diffuse;   	float dummy3;
    vec3 specular;  	float dummy4;
};

struct SpotLight {
    vec3 position;  float dummy0;
    vec3 direction; float dummy1;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient; 
    vec3 diffuse;    float dummy2;
    vec3 specular;   float dummy3;
};


in vec3 FragPos;
in vec3 Normal;
// in vec2 TexCoords;


uniform vec3 viewPos;
layout (std140) uniform DirLightUBO {
    DirLight dirLight;
};


// #define NR_POINT_LIGHTS 1
// layout (std140) uniform PointLightsUBO {
//     PointLight pointLights[NR_POINT_LIGHTS];
// };
// layout (std140) uniform SpotLightUBO {
//     SpotLight spotLight;
// };
uniform Material material;
// uniform sampler2D texture_diffuse1;

uniform bool blinn;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // 1: directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // no need for this scene now, so i commented it out for now
    // 2: point lights
    // for(int i = 0; i < NR_POINT_LIGHTS; i++)
    //     result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    // // 3: spot light
    // result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
  // check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        BrightColor = vec4(result, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

        // emission
    BrightColor = vec4(result, 1.0) *0.7;
    FragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    // vec3 reflectDir = reflect(-lightDir, normal);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float spec = 0.0;
    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = max(pow(max(dot(normal, halfwayDir), 0.0), 32.0), 0.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = max(pow(max(dot(viewDir, reflectDir), 0.0), 8.0), 0.0);
    }

    // combine results
    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
    // return ambient;
    // return vec3(1.0, 1.0, 1.0);
    // return vec3 (spec, spec, spec);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec = 0.0;
    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading 
    float spec = 0.0;
    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}