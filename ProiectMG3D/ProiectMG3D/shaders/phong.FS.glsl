#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    int shininess;
    sampler2D tex;
    int useTexture;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 color;
};

in vec3 world_position;
in vec3 world_normal;
in vec2 texCoord;

out vec4 frag_color;

uniform Material material;
uniform vec3 ambient_color;

uniform int point_lights_count;
uniform vec3 point_light_positions[10];
uniform vec3 point_light_colors[10];

uniform int spot_lights_count;
uniform vec3 spot_light_positions[10];
uniform vec3 spot_light_colors[10];
uniform vec3 spot_light_directions[10];
uniform float spot_light_angles[10];

uniform vec3 camera_position;

vec4 color;

vec3 ComputePhongIllumination(vec3 light_position)
{
    vec3 N = normalize(world_normal);
    vec3 L = normalize(light_position - world_position);
    vec3 R = normalize(reflect(-L, N));
    vec3 V = normalize(camera_position - world_position);

    vec3 diffuse_component = (material.useTexture == 1 ? color.rgb : material.diffuse) * max(dot(N, L), 0);

    vec3 specular_component = vec3(0);
    if (length(diffuse_component) > 0) {
        specular_component = (material.useTexture == 1 ? color.rgb : material.specular) * pow(max(dot(V, R), 0), material.shininess);
    }

    vec3 illumination = diffuse_component + specular_component;

    return illumination;
}

vec3 ComputeAmbientComponent()
{
    vec3 ambient_component = (material.useTexture == 1 ? color.rgb : material.ambient) * ambient_color;

    return ambient_component;
}

float ComputeDistanceAttenuation(vec3 light_position, vec3 point_position)
{
    float dist = distance(light_position, point_position);
    return 1.0 / (dist * dist + 1);
}

vec3 ComputePointLightSourcesIllumination()
{
    vec3 lights_illumination = vec3(0);

    for (int i=0;i<point_lights_count;i++) {
        vec3 light_position = point_light_positions[i];
        vec3 light_color = point_light_colors[i];

        vec3 light_illumination = ComputePhongIllumination(light_position);
        float illumination_attenuation = ComputeDistanceAttenuation(light_position, world_position);

        lights_illumination += light_illumination * illumination_attenuation * light_color;
    }

    return lights_illumination;
}

vec3 ComputeSpotLightSourcesIllumination()
{
    vec3 lights_illumination = vec3(0);

    for (int i=0;i<spot_lights_count;i++) {
        vec3 light_position = spot_light_positions[i];
        vec3 light_color = spot_light_colors[i];
        vec3 light_direction = spot_light_directions[i];
        float cut_off_angle = spot_light_angles[i];

        vec3 L = normalize(light_position-world_position);
        float cos_theta_angle = dot(-L, light_direction);
        float cos_phi_angle = cos(cut_off_angle);
        if (cos_theta_angle > cos_phi_angle)
        {
            vec3 light_illumination = ComputePhongIllumination(light_position);
            float illumination_attenuation = ComputeDistanceAttenuation(light_position, world_position);

            float spot_linear_att_factor = (cos_theta_angle - cos_phi_angle) / (1.0f - cos_phi_angle);
            float quadratic_spot_light_att_factor = spot_linear_att_factor * spot_linear_att_factor;

            lights_illumination += light_illumination * illumination_attenuation * quadratic_spot_light_att_factor * light_color;
        }
    }

    return lights_illumination;
}

void main()
{
    if (material.useTexture == 1) { 
        color = texture2D(material.tex, texCoord);
        if (color.a < 0.5)
            discard;
    }
    vec3 illumination = ComputePointLightSourcesIllumination()
                      + ComputeSpotLightSourcesIllumination()
                      + ComputeAmbientComponent();
    frag_color = vec4(illumination, 1.0);
} 