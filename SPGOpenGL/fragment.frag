#version 330 core

out vec4 FragColor;
uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;
uniform sampler2D texture_normal;
//float shininess;




struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

struct Material {
    vec3 Diffuse;
    vec3 Specular;
    vec3 Ambient;
    float Shininess;
};

#define NR_POINT_LIGHTS 2

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

//----------------------- material used if text not set -------------------
uniform Material material;
uniform bool txtDiffuse;
uniform bool txtSpecular;
uniform bool txtNormal;


//------------------------------------------------------------------------
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform SpotLight spotLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];


uniform bool flashOn;
uniform bool pointLightOn[NR_POINT_LIGHTS];
uniform bool directionalLightOn;
uniform bool blinn;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{    
   

    vec3 norm;
    if(txtNormal){
        norm =texture(texture_normal, TexCoords).rgb;
        norm.g = 1 - norm.g;
	    norm = norm * 2 - 1;
	
    }else{
        norm = normalize(Normal);
    }

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0f);

    // directional lighting
    if(directionalLightOn)
        result = CalcDirLight(dirLight, norm, viewDir);

    // point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        if(pointLightOn[i])
            result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);  
        
    if(flashOn)
        result += CalcSpotLight(spotLight, norm, FragPos, viewDir); 


   
    float gamma = 2.2;
    //FragColor =  vec4(pow(result, vec3(1.0/gamma)),1.f);
     FragColor =  vec4(result,1.f);
}





vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);


    float spec;
    if(blinn)
    {
        //Blind fong reflexion
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.Shininess);
    }
    else
    {
        //Fong reflexion
      vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    }
   
    vec3 tDiffuse=texture(texture_diffuse, TexCoords).rgb;
    vec3 tSpecular=texture(texture_specular, TexCoords).rgb;

    vec3 ambient,diffuse,specular;
    if(txtDiffuse){
        ambient = light.ambient * texture(texture_diffuse, TexCoords).rgb;
        diffuse = light.diffuse * diff * texture(texture_diffuse, TexCoords).rgb;
    }else{
        ambient = light.ambient * material.Ambient;
        diffuse = light.diffuse * diff * material.Diffuse;
    }
    if(txtSpecular)
        specular = light.specular * spec * texture(texture_specular, TexCoords).rgb;
    else
        specular = light.specular * spec * material.Specular;
    return (ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    

     float spec;
    if(blinn)
    {
        //Blind fong reflexion
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.Shininess);
    }
    else
    {
        //Fong reflexion
      vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    }

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (1 + light.linear * dist + light.quadratic * (dist * dist));    
    
   vec3 ambient,diffuse,specular;
    if(txtDiffuse){
        ambient = light.ambient * texture(texture_diffuse, TexCoords).rgb;
        diffuse = light.diffuse * diff * texture(texture_diffuse, TexCoords).rgb;
    }else{
        ambient = light.ambient * material.Ambient;
        diffuse = light.diffuse * diff * material.Diffuse;
    }
    if(txtSpecular)
        specular = light.specular * spec * texture(texture_specular, TexCoords).rgb;
    else
        specular = light.specular * spec * material.Specular;

        
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
   
    float spec;
    if(blinn)
    {
        //Blind fong reflexion
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.Shininess);
    }
    else
    {
        //Fong reflexion
      vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Shininess);
    }

    // attenuation
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (1 + light.linear * dist + light.quadratic * (dist * dist));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient,diffuse,specular;
    if(txtDiffuse){
        ambient = light.ambient * texture(texture_diffuse, TexCoords).rgb;
        diffuse = light.diffuse * diff * texture(texture_diffuse, TexCoords).rgb;
    }else{
        ambient = light.ambient * material.Ambient;
        diffuse = light.diffuse * diff * material.Diffuse;
    }
    if(txtSpecular)
        specular = light.specular * spec * texture(texture_specular, TexCoords).rgb;
    else
        specular = light.specular * spec * material.Specular;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}
