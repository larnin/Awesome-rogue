uniform sampler2D primaryTexture;
uniform sampler2D secondaryTexture; //normal map
uniform vec3[100] light;
uniform vec4[100] lightColor;
uniform float[100] lightType; // 0=point, 1=spot, 2=directional
uniform vec4[100] lightParams; // x=radius, y=intensity, z=yaw, w=pitch
uniform int lightCount;
uniform vec4 ambiantColor;
uniform vec4 material; // x=ambiant, y=diffuse, z=specular, w=specularMultiplier

varying vec4 position;

vec3 texToNormal(vec3 vec)
{
    return normalize(vec3(-(vec.x-0.5), vec.y-0.5, -(vec.z-0.5)));
}

void point(vec3 normal, int i, out vec4 diffuse, out vec4 specular)
{
    vec3 delta = vec3(position.xyz - light[i]);

    float l = length(delta.xy);
    l = (lightParams[i].x - l) / lightParams[i].x;
    if(l < 0.0)
        return;

    vec3 R = normalize(reflect(delta, normal));
    vec3 V = vec3(0, 0, 1);

    diffuse = max(0.0, dot(normal, normalize(delta))) * material.y * lightColor[i] * l * lightParams[i].y;
    specular = max(0.0, pow(dot(R, V), material.w)) * material.z * lightColor[i] * l * lightParams[i].y;
}

void spot(vec3 normal, int i, out vec4 diffuse, out vec4 specular)
{
    if(lightParams[i].w <= 0.0)
        return;

    vec3 delta = vec3(position.xyz - light[i]);
    vec2 dir = vec2(cos(lightParams[i].z), sin(lightParams[i].z));
    float angle = acos(dot(normalize(dir), normalize(delta.xy)));

    float l = length(delta.xy);
    l = (lightParams[i].x - l) / lightParams[i].x;
    if(l < 0.0)
        return;
    float m = (lightParams[i].w - abs(angle)) / lightParams[i].w;
    if(m < 0.0)
        return;

    vec3 R = normalize(reflect(delta, normal));
    vec3 V = vec3(0, 0, 1);

    diffuse = max(0.0, dot(normal, normalize(delta))) * material.y * lightColor[i] * l * m * lightParams[i].y;
    specular = max(0.0, pow(dot(R, V), material.w)) * material.z * lightColor[i] * l * m * lightParams[i].y;
}

void directional(vec3 normal, int i, out vec4 diffuse, out vec4 specular)
{
    vec3 delta = vec3(cos(lightParams[i].z)*cos(lightParams[i].w), sin(lightParams[i].z)*cos(lightParams[i].w), sin(lightParams[i].w));

    vec3 R = normalize(reflect(delta, normal));
    vec3 V = vec3(0, 0, 1);

    diffuse = max(0.0, dot(normal, normalize(delta))) * material.y * lightColor[i] * lightParams[i].y;
    if(delta.z < 0)
        specular = max(0.0, pow(dot(R, V), material.w)) * material.z * lightColor[i] * lightParams[i].y;
}

void main()
{
    vec4 pixel = texture2D(primaryTexture, gl_TexCoord[0].xy);
    vec3 normal = texToNormal(texture2D(secondaryTexture, gl_TexCoord[0].xy).xyz);

    vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);

    vec4 diffuseTempColor = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 specularTempColor = vec4(0.0, 0.0, 0.0, 0.0);

    for(int i = 0 ; i < lightCount ; i++)
    {
        if(lightType[i] <= 0.5)
            point(normal, i, diffuseTempColor, specularTempColor);
        else if(lightType[i] <= 1.5)
            spot(normal, i, diffuseTempColor, specularTempColor);
        else if(lightType[i] <= 2.5)
            directional(normal, i, diffuseTempColor, specularTempColor);
        diffuseColor += diffuseTempColor;
        specularColor += specularTempColor;

        diffuseTempColor = vec4(0, 0, 0, 0);
        specularTempColor = vec4(0, 0, 0, 0);
    }

    specularColor = vec4(specularColor.xyz, 0);

    gl_FragColor = gl_Color * (pixel * (ambiantColor * material.x + diffuseColor)) + specularColor;
    gl_FragColor.w = pixel.w;
}
