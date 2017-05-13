uniform sampler2D primaryTexture;
uniform sampler2D secondaryTexture; //normal map
uniform vec3[100] light;
uniform vec4[100] lightColor;
uniform float[100] lightRadius;
uniform int lightCount;
uniform vec4 ambiantColor;
uniform vec4 material; // x=ambiant, y=diffuse, z=specular, w=specularMultiplier

varying vec4 position;

vec3 texToNormal(vec3 vec)
{
    return normalize(vec3(-(vec.x-0.5), vec.y-0.5, -(vec.z-0.5)));
}

void main()
{
    vec4 pixel = texture2D(primaryTexture, gl_TexCoord[0].xy);
    vec3 normal = texToNormal(texture2D(secondaryTexture, gl_TexCoord[0].xy).xyz);

    vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);

    for(int i = 0 ; i < lightCount ; i++)
    {
        vec3 delta = vec3(position.xyz - light[i]);

        float l = length(delta.xy);
        l = lightRadius[i] - l;
        if(l < 0.0)
            continue;
        l /= lightRadius[i];


        vec3 R = normalize(reflect(delta, normal));
        vec3 V = vec3(0, 0, 1);

        diffuseColor += max(0.0, dot(normal, normalize(delta))) * material.y * lightColor[i] * l;
        specularColor += max(0.0, pow(dot(R, V), material.w)) * material.z * lightColor[i] * l;
        /*vec4 diffuse = max(0.0, dot(normal, normalize(delta))) * material.y * lightColor[i] * l;
        vec4 specular = max(0.0, pow(dot(R, V), material.w)) * material.z * lightColor[i] * l;
        color.r = max(max(color.r, diffuse.r), specular.r);
        color.g = max(max(color.g, diffuse.g), specular.g);
        color.b = max(max(color.b, diffuse.b), specular.b);
        color.a = max(max(color.a, diffuse.a), specular.a);*/
    }

    specularColor = vec4(specularColor.xyz, pixel.w);

    gl_FragColor = gl_Color * pixel * (ambiantColor * material.x + diffuseColor) + specularColor;
}
