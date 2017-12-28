#include "material.h"
#include <QDataStream>
#include <QFile>
#include "src/Editor/Utils/foldergestion.h"

QString Material::DefaultSourceFragmentShader =
        "#version 330 core\n"
        "in VS_OUT{\n"
        "    vec3 normal;\n"
        "    vec3 toLightVector;\n"
        "    vec3 toLightVectorTangent;\n"
        "    vec3 toCameraVector;\n"
        "    vec3 toCameraVectorTangent;\n"
        "    vec2 textureCoord;\n"
        "    vec3 viewDir;\n"
        "    mat3 RM;\n"
        "    mat3 TBN;\n"
        "}fs_in;\n"
        "out vec4 FragColor;\n"
        "uniform vec3 lightPosition;\n"
        "uniform vec3 lightTint;\n"
        "uniform vec3 viewPos;\n"
        "uniform float time;\n"
        "layout(binding=0) uniform samplerCube irradianceMap;\n"
        "layout(binding=1) uniform samplerCube prefilterMap;\n"
        "layout(binding=2) uniform sampler2D brdfLUT;\n"
        "//#toReplaceUniform\n"
        "//#toReplaceFunctions\n"
        "void main(void){\n"
       "    bool normalMapped = false;\n"
       "    float roughness=0;\n"
       "    float metallic=0;\n"
       "    float ao = 1;\n"
       "    vec4 BaseColor=vec4(1);\n"
       "    vec4 normalValue = vec4(0);\n"

        "//#toReplace\n"

       "    vec3 albedo = pow(BaseColor.rgb, vec3(2.2));"
       "    vec3 unitLightVector;\n"
       "    vec3 unitVectorToCamera = normalize(fs_in.toCameraVector);\n"
       "    vec3 unitNormal;\n"

       "    unitNormal = normalize(fs_in.normal);\n"
       " if(normalMapped){\n"
       "     unitLightVector = normalize(fs_in.toLightVectorTangent);\n"
       "     unitVectorToCamera = normalize(fs_in.toCameraVectorTangent);\n"
       "     unitNormal = normalize(normalValue.rgb);\n"
       "}else{\n"
       "     unitLightVector = normalize(fs_in.toLightVector);\n"
       "     unitVectorToCamera = normalize(fs_in.toCameraVector);\n"
       "     unitNormal = normalize(fs_in.normal);\n"
        "}\n"
        "unitVectorToCamera = normalize(fs_in.toCameraVector);\n"

        "   if(normalMapped){\n"
        "   	unitNormal = normalize(fs_in.RM * (unitNormal * fs_in.TBN))\n;"
        "   }\n"

        "   vec3 reflectNormal = reflect(-unitVectorToCamera, unitNormal);\n"

        "   vec3 F0 = vec3(0.04);\n"
        "   F0 = mix(F0, albedo, metallic);\n"

        "   // L0 = reflectance equation\n"
        "   vec3 L0 = vec3(0.0);\n"
        "   unitLightVector = normalize(fs_in.toLightVector);\n"
        "   vec3 unitHalfway = normalize(unitVectorToCamera + unitLightVector);\n"


        "   vec3 radiance = lightTint;\n"

        "   // Cook-Torrance Term BRDF\n"
        "   float NDF = DistributionGGX(unitNormal, unitHalfway, roughness);\n"
        "   float G   = GeometrySmith(unitNormal, unitVectorToCamera, unitLightVector, roughness);\n"
        "   vec3 F    = fresnelSchlick(max(dot(unitHalfway, unitVectorToCamera), 0.0), F0);\n"

        "   vec3 nominator    = NDF * G * F;\n"
        "   float denominator = 4 * max(dot(unitNormal, unitVectorToCamera), 0.0) * max(dot(unitNormal, unitLightVector), 0.0) + 0.001; // 0.001 to prevent divide by zero.\n"
        "   vec3 specular = nominator / denominator;\n"

        "   // Specular amount\n"
        "   vec3 kS = F;\n"
        "   // Diffuse amount\n"
        "   vec3 kD = vec3(1.0) - kS;\n"
        "   kD *= 1.0 - metallic;\n"


        "   float brightNess = max(dot(unitNormal, unitLightVector), 0.0);\n"
        "   vec3 emittance = radiance * brightNess;\n"
        "   L0 += (kD * albedo / PI + specular) * emittance;\n"

        "   F = fresnelSchlickRoughness(max(dot(unitNormal, unitVectorToCamera), 0.0), F0, roughness);\n"

        "   kS = F;\n"
        "   kD = 1.0 - kS;\n"
        "   kD *= 1.0 - metallic;\n"

        "   vec3 irradiance = texture(irradianceMap, unitNormal).rgb;\n"
        "   vec3 diffuse = irradiance * albedo;\n"

        "   const float MAX_REFLECTION_LOD = 4.0;\n"
        "   vec3 prefilteredColor = textureLod(prefilterMap, reflectNormal,  roughness * MAX_REFLECTION_LOD).rgb;\n"
        "   vec2 brdf  = texture(brdfLUT, vec2(max(dot(unitNormal, unitVectorToCamera), 0.0), roughness)).rg;\n"
        "   specular = prefilteredColor * (F * brdf.x + brdf.y);\n"

        "   vec3 ambient = (kD * diffuse + specular) * ao;\n"

        "   vec3 color = ambient + L0;\n"

        "   // tonemapping HDR\n"
        "   color = color / (color + vec3(1.0));\n"
        "   //correction gamma\n"
        "   color = pow(color, vec3(1.0/2.2));\n"

        "   FragColor = vec4(color , 1.0);\n"
    "}\n";

/*QString Material::DefaultSourceFragmentShader =
    "#version 330 core\n"
    "in VS_OUT{\n"
    "   vec3 normal;\n"
    "   vec3 toLightVector;\n"
    "   vec3 toLightVectorTangent;\n"
    "   vec3 toCameraVector;\n"
    "   vec3 toCameraVectorTangent;\n"
    "   vec2 textureCoord;\n"
    "   vec3 viewDir;\n"
    "   mat3 RM;\n"
    "   mat3 TBN;\n"
    "   vec3 worldPos;\n"
    "}fs_in;\n"

    "layout (location = 0) out vec3 position;\n"
    "layout (location = 1) out vec3 normal;\n"
    "layout (location = 2) out vec4 albedoRoughness;\n"
    "layout (location = 3) out vec4 metalOcclusion;\n"

    "//#toReplaceUniform\n"
    "//#toReplaceFunctions\n"
    "void main(void){\n"
   "    bool normalMapped = false;\n"
   "    float roughness=0;\n"
   "    float metallic=0;\n"
   "    float ao = 1;\n"
   "    vec4 BaseColor=vec4(1);\n"
   "    vec3 normalValue = fs_in.normal;\n"

    "//#toReplace\n"
    "   position = fs_in.worldPos;\n"
    "   normal = normalize(normalValue);\n"
    "   albedoRoughness.rgb = BaseColor.rgb;\n"
    "   albedoRoughness.a = roughness;\n"
    "   metalOcclusion.r = metallic;\n"
    "   metalOcclusion.g = ao;\n"


"}\n";*/

/*Init Vertex Shader*/
QString Material::DefaultSourceVertexShader = "#version 330 core\n"

        "in vec3 position;\n"
        "in vec2 textureCoord;\n"
        "in vec3 normal;\n"
        "in vec3 tangent;\n"

        "out VS_OUT{\n"
        "vec3 normal;\n"
        "vec3 toLightVector;\n"
        "vec3 toLightVectorTangent;\n"
        "vec3 toCameraVector;\n"
        "vec3 toCameraVectorTangent;\n"
        "vec2 textureCoord;\n"
        "vec3 viewDir;\n"
        "mat3 RM;\n"
        "mat3 TBN;\n"
        "}vs_out;\n"

        "uniform mat4 projectionMatrix;\n"
        "uniform mat4 modelMatrix;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform vec3 viewPos;\n"
        "uniform vec3 lightPosition;\n"

        "void main(void)\n"
        "{\n"
        "  vec4 worldPosition = modelMatrix * vec4(position,1.0);\n"
        "  vs_out.normal = (((modelMatrix)) * vec4(normal,0.0)).xyz;\n"
        "  vec3 T = normalize( tangent);\n"
        "  vec3 N = normalize(normal);\n"
        "  vec3 B = normalize(cross(N,T));\n"
        "  vs_out.TBN = mat3(T.x, B.x, N.x,T.y, B.y, N.y,T.z, B.z, N.z);\n"
        "  vec3 TangentLightPos =  vs_out.TBN * lightPosition;\n"
        "  vec3 TangentViewPos  =  vs_out.TBN * viewPos;\n"
        "  vec3 TangentFragPos  =  vs_out.TBN * worldPosition.xyz;\n"
        "  gl_Position = projectionMatrix * viewMatrix * worldPosition;\n"
        "  vs_out.toLightVector =  (lightPosition - worldPosition.xyz);\n"
        "  vs_out.toLightVectorTangent = TangentLightPos - TangentFragPos;\n"
        "  vs_out.toCameraVector = (inverse(viewMatrix) * vec4(0.0,0.0,0.0,1.0)).xyz - worldPosition.xyz;\n"
        "  vs_out.toCameraVectorTangent = TangentViewPos - TangentFragPos;\n"

        "  vs_out.textureCoord = textureCoord;\n"
        "  vs_out.viewDir = -worldPosition.xyz;\n"
        "  vs_out.viewDir = vs_out.TBN * vs_out.viewDir;\n"
        "    vs_out.RM = mat3( modelMatrix[0].xyz,\n"
        "  modelMatrix[1].xyz,\n"
        "  modelMatrix[2].xyz );\n"
        "}\n";

Material::Material( )
    :Asset(-1, "name", "path")
{

}

Material::Material(unsigned int handle, const std::string name, std::string path)
    :Asset(handle, name, path)
{
    try{
    QFile archiveFile;
    archiveFile.setFileName(QString(path.c_str()));
    if (!archiveFile.open(QIODevice::ReadOnly))
        return;

    QDataStream dataStream;
    dataStream.setDevice(&archiveFile);
    bool init;
    QString vS,fS;
    dataStream >> init;
    if(init){
        dataStream >> vS;
        dataStream >> fS;

        m_vertexShader = vS.toStdString();
        m_fragmentShader = fS.toStdString();

        size_t size;
        dataStream >> size;
        for(int i=0;i<size;i++){
            QString path;
            dataStream >> path;
            path = FolderGestion::checkoutReferences(path);
            m_texturespath.push_back(path.toStdString());
            unsigned int handle = AssetsCollections::addTexture(path.toStdString(),path.toStdString());
        }

        m_shader.initWithSource(vS,fS);
    }else{
        m_shader.initWithSource(DefaultSourceVertexShader,DefaultSourceFragmentShader);
    }


    m_shaderInit = true;

    archiveFile.close();
    }catch(const std::exception &e){
        std::cerr << "Exception rise in Material Loader " << name << " " << e.what() << std::endl;
    }
}

Material::~Material()
{
    m_shader.cleanUp();
}

void Material::newEmptyMat(QString path)
{
    QFile archiveFile;
    archiveFile.setFileName(path);
    if (!archiveFile.open(QIODevice::WriteOnly))
        return;

    QDataStream dataStream;
    dataStream.setDevice(&archiveFile);

    dataStream << false;

    archiveFile.close();
    return;
}

bool Material::isShaderInit()
{
    return m_shaderInit;
}

Shader Material::getShader()
{
    return m_shader;
}

std::vector<std::string> Material::getTexturesPath()
{
    return m_texturespath;
}
