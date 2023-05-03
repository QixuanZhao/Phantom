#version 460 core

layout(pixel_center_integer) in vec4 gl_FragCoord;

uniform sampler2D input_position;
uniform sampler2D input_depth;
uniform samplerCubeShadow depthMap;

uniform vec3 lightPosition;
uniform float far_plane;

out float red;

uniform bool samplingEnabled = true;
uniform int samplingMethod = 0;
uniform float samplingSpread = 3.0;
uniform int filterSize = 1;
uniform bool samplingStratified = true;

vec2 computeHorizontalFromDirection(in vec3 direction);
vec3 computeDirectionFromHorizontal(in vec2 horizontal);

float random(vec4 seed4) {
    float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
    return fract(sin(dot_product) * 43758.5453123);
}

float pcf (vec2 horizontal, float currentDepth);
float pds (vec2 horizontal, float currentDepth);

const vec2 poissonDisk[] = vec2[] (
    vec2(-2.43813288e-01, -2.40604147e-01),
    vec2( 1.06442024e-01, -3.88435871e-01),
    vec2(-6.33279391e-01, -1.35094075e-01),
    vec2(-3.33686105e-01,  3.10284481e-01),
    vec2(-1.00272418e-01, -8.31321379e-01),
    vec2(-8.20041514e-01,  3.98520693e-01),
    vec2( 4.89757882e-01, -6.40634807e-01),
    vec2(-8.04120919e-04,  6.39870007e-01),
    vec2( 1.65814980e-01, -2.26199817e-02),
    vec2( 5.67002455e-01, -1.18190332e-01),
    vec2(-5.01726000e-01, -5.20164694e-01),
    vec2( 2.86887066e-01,  8.60209386e-01),
    vec2( 3.38253647e-01,  4.09842379e-01),
    vec2( 7.35903313e-01,  3.52437905e-01),
    vec2( 2.45980556e-01, -9.29254806e-01),
    vec2( 9.16596184e-01,  3.59089728e-02)
);

const vec2 poissonDiskLarge[] = vec2[] (
    vec2(-0.79134878, -0.58091925),
    vec2(-0.62625411, -0.73347638),
    vec2(-0.63182169, -0.63129382),
    vec2(-0.66882557, -0.49754905),
    vec2(-0.55302672, -0.80723528),
    vec2(-0.50544333, -0.67873848),
    vec2(-0.57971869, -0.47078858),
    vec2(-0.53710033, -0.3331768 ),
    vec2(-0.83706343, -0.46169196),
    vec2(-0.47654182, -0.55582449),
    vec2(-0.74232399, -0.42058605),
    vec2(-0.40151044, -0.80410022),
    vec2(-0.2395068 , -0.7956897 ),
    vec2(-0.88944887, -0.38775255),
    vec2(-0.63006295, -0.33875784),
    vec2(-0.33668572, -0.87325195),
    vec2(-0.23459916, -0.91800856),
    vec2(-0.71930929, -0.29117686),
    vec2(-0.83307181, -0.30533389),
    vec2(-0.50199562, -0.17806001),
    vec2(-0.39765941, -0.10009739),
    vec2(-0.93720003, -0.29895883),
    vec2(-0.62695718, -0.13121097),
    vec2(-0.34238327, -0.72830929),
    vec2(-0.36412382, -0.3060347 ),
    vec2(-0.63459728, -0.02680315),
    vec2(-0.38032728, -0.56498281),
    vec2(-0.15749447, -0.64097891),
    vec2(-0.72561158, -0.13363306),
    vec2(-0.44954594, -0.40443473),
    vec2(-0.52520089, -0.0806658 ),
    vec2(-0.90545979, -0.15707507),
    vec2(-0.29105937, -0.64444129),
    vec2(-0.27908289, -0.5431838 ),
    vec2(-0.33430692, -0.43695449),
    vec2(-0.55111487,  0.07169583),
    vec2(-0.15817132, -0.55057481),
    vec2(-0.39091765,  0.0030728 ),
    vec2(-0.80234459, -0.02721489),
    vec2(-0.8189337 ,  0.07489734),
    vec2(-0.41689175, -0.65881324),
    vec2(-0.22294237, -0.05163606),
    vec2(-0.09163165, -0.04679756),
    vec2(-0.70205547,  0.09210689),
    vec2(-0.78471523, -0.22068526),
    vec2(-0.02601755, -0.6427596 ),
    vec2( 0.14658089, -0.64490676),
    vec2(-0.42085181, -0.2382466 ),
    vec2(-0.11683594, -0.43448456),
    vec2( 0.01658275, -0.06402021),
    vec2(-0.66171568,  0.25646044),
    vec2(-0.1570713 , -0.85330138),
    vec2(-0.63286635, -0.22415941),
    vec2(-0.13284787, -0.76197066),
    vec2(-0.79271687,  0.22502248),
    vec2(-0.01909845, -0.55373835),
    vec2(-0.21199787, -0.47535616),
    vec2( 0.1639267 , -0.13766342),
    vec2(-0.18598393, -0.34788264),
    vec2(-0.42791521,  0.18855916),
    vec2(-0.34560181,  0.11125705),
    vec2(-0.11575423, -0.24990267),
    vec2(-0.0665121 , -0.91603302),
    vec2(-0.2754914 , -0.36017012),
    vec2(-0.57333029,  0.19810629),
    vec2(-0.03820775, -0.7639599 ),
    vec2(-0.89295637,  0.01281761),
    vec2(-0.46036132,  0.09315545),
    vec2( 0.03138262, -0.84130071),
    vec2(-0.29623861, -0.14368958),
    vec2( 0.04825501, -0.70476449),
    vec2( 0.07255197, -0.97993596),
    vec2(-0.26963684, -0.26537815),
    vec2(-0.12614859, -0.14031827),
    vec2(-0.03144223, -0.33980469),
    vec2( 0.06362349, -0.59949179),
    vec2( 0.1941354 , -0.86241743),
    vec2( 0.26452814, -0.015682  ),
    vec2(-0.52896474,  0.35567559),
    vec2(-0.98390412,  0.05476114),
    vec2(-0.31701327,  0.27256218),
    vec2(-0.18255372,  0.11288508),
    vec2(-0.96934127, -0.03328936),
    vec2(-0.67624481,  0.38822883),
    vec2( 0.29728106, -0.13599567),
    vec2( 0.12660565, -0.79359419),
    vec2(-0.78357916,  0.36988508),
    vec2( 0.02391119, -0.15917432),
    vec2( 0.18474967, -0.22369888),
    vec2( 0.03386174, -0.26830185),
    vec2(-0.01692587, -0.99556206),
    vec2( 0.20475404, -0.49706871),
    vec2(-0.17363591,  0.24829786),
    vec2(-0.06383523,  0.14565565),
    vec2(-0.89081347,  0.33528079),
    vec2(-0.01405642,  0.0708918 ),
    vec2(-0.25775788,  0.03004338),
    vec2( 0.10000644, -0.47320244),
    vec2( 0.29218875, -0.40220643),
    vec2(-0.21793381, -0.18811387),
    vec2( 0.09308851, -0.36744357),
    vec2( 0.42374971, -0.04508693),
    vec2(-0.24744483,  0.18913454),
    vec2( 0.33075779, -0.50292615),
    vec2(-0.1042179 ,  0.40750076),
    vec2( 0.2148508 , -0.58779695),
    vec2( 0.32116531, -0.92187661),
    vec2( 0.55291386,  0.00753142),
    vec2(-0.43027009,  0.33831479),
    vec2( 0.28128005, -0.30557661),
    vec2(-0.19929104,  0.35996368),
    vec2(-0.03870898,  0.26011484),
    vec2(-0.49898297,  0.25296238),
    vec2(-0.23068483,  0.50982213),
    vec2(-0.33181286,  0.45369643),
    vec2( 0.19880129, -0.38268023),
    vec2( 0.35232704, -0.65823676),
    vec2( 0.41152526,  0.0497852 ),
    vec2(-0.9442313 ,  0.13806793),
    vec2( 0.07699146,  0.23245591),
    vec2( 0.01976402,  0.36906754),
    vec2( 0.11343494, -0.03545477),
    vec2(-0.21936181,  0.64353394),
    vec2( 0.04073676,  0.45709295),
    vec2( 0.44382946, -0.30808879),
    vec2( 0.44588463, -0.45904147),
    vec2( 0.12040956,  0.08894055),
    vec2( 0.31342784, -0.76154382),
    vec2(-0.10505862,  0.55683811),
    vec2( 0.359071  , -0.22968757),
    vec2(-0.82161671,  0.4581846 ),
    vec2(-0.57551003,  0.45472024),
    vec2(-0.45536791,  0.43326216),
    vec2( 0.03793567,  0.14311624),
    vec2(-0.33966223,  0.36068757),
    vec2( 0.00857676,  0.62999111),
    vec2(-0.35593784,  0.67240794),
    vec2( 0.24037886,  0.17582504),
    vec2(-0.05249817,  0.70848414),
    vec2(-0.13861501,  0.67956473),
    vec2(-0.68471776,  0.54300298),
    vec2(-0.42074209,  0.57197156),
    vec2( 0.17061959,  0.34082825),
    vec2( 0.29332661,  0.32846084),
    vec2( 0.17043675,  0.46663734),
    vec2(-0.10671691,  0.06138343),
    vec2(-0.11301979,  0.31292044),
    vec2( 0.44040491, -0.15517893),
    vec2( 0.52652411, -0.17821835),
    vec2(-0.01013519,  0.53491014),
    vec2(-0.15904441,  0.81082824),
    vec2( 0.41264123, -0.54096136),
    vec2( 0.00944816,  0.77471496),
    vec2(-0.26590275,  0.76337973),
    vec2( 0.01380191, -0.43220887),
    vec2( 0.54406101, -0.51260079),
    vec2( 0.34337781,  0.17158393),
    vec2( 0.60168992, -0.43105415),
    vec2(-0.44482664,  0.74198756),
    vec2( 0.17963585,  0.24473944),
    vec2( 0.61965948, -0.3202223 ),
    vec2( 0.3638718 , -0.34578995),
    vec2(-0.27059264,  0.85880741),
    vec2(-0.56170951,  0.57820284),
    vec2( 0.21939442, -0.77277208),
    vec2(-0.47119493,  0.64609058),
    vec2( 0.52906294, -0.37288531),
    vec2( 0.39472352, -0.85131646),
    vec2( 0.23659694,  0.08449987),
    vec2(-0.30793895,  0.58948908),
    vec2( 0.48293295, -0.71314056),
    vec2( 0.10265913, -0.89512206),
    vec2( 0.13746986,  0.56494606),
    vec2( 0.66898763, -0.49406649),
    vec2( 0.6860983 , -0.19132596),
    vec2(-0.03354102,  0.91474041),
    vec2( 0.50000424,  0.11692862),
    vec2(-0.66077325,  0.66167482),
    vec2( 0.39712284,  0.31447638),
    vec2( 0.32906999,  0.41176137),
    vec2( 0.64149986, -0.72327407),
    vec2( 0.74315171, -0.3946264 ),
    vec2( 0.54618704, -0.80030479),
    vec2( 0.61373527, -0.6210237 ),
    vec2( 0.24758555, -0.6844573 ),
    vec2( 0.72951562, -0.56248382),
    vec2(-0.19360182,  0.90527857),
    vec2(-0.6015814 ,  0.75984197),
    vec2( 0.45478495,  0.41371684),
    vec2( 0.61437348, -0.06150267),
    vec2( 0.42774472,  0.21648583),
    vec2( 0.25612877,  0.62850505),
    vec2( 0.06587984,  0.90202269),
    vec2( 0.18254617,  0.74688811),
    vec2( 0.29120048,  0.79511977),
    vec2( 0.81672642, -0.50712338),
    vec2( 0.72010627, -0.02131702),
    vec2( 0.21617424,  0.93853565),
    vec2(-0.73294382,  0.45874755),
    vec2( 0.59298191,  0.19847754),
    vec2( 0.20486157,  0.8340489 ),
    vec2( 0.34136221,  0.50278161),
    vec2(-0.50040567,  0.51373843),
    vec2(-0.39564703,  0.81758691),
    vec2( 0.8279437 , -0.09725028),
    vec2( 0.41455016,  0.69080953),
    vec2( 0.12994403,  0.96997894),
    vec2( 0.12660928,  0.67513079),
    vec2( 0.42004668,  0.56373587),
    vec2( 0.71618503,  0.07048252),
    vec2( 0.71891681, -0.30923219),
    vec2( 0.82398287, -0.2648619 ),
    vec2( 0.75287109, -0.65698466),
    vec2( 0.54059132,  0.35517538),
    vec2( 0.90980051, -0.34017464),
    vec2( 0.32534546,  0.71276467),
    vec2( 0.65567206,  0.30732984),
    vec2( 0.94623975,  0.18942574),
    vec2( 0.78770487,  0.3665491 ),
    vec2( 0.56188836,  0.70950839),
    vec2( 0.461108  ,  0.86841177),
    vec2( 0.60160826,  0.10322478),
    vec2( 0.94464939,  0.32641435),
    vec2( 0.41031795,  0.78370742),
    vec2( 0.70640339,  0.60433708)
);

void main() {
    ivec2 FragCoord = ivec2(gl_FragCoord.xy);
    float depth = texelFetch(input_depth, FragCoord, 0).r;
    if (depth == 1.0) {
        red = 0.0;
        return;
    }

    vec3 fragmentPosition = texelFetch(input_position, FragCoord, 0).rgb;

    vec3 lightToFrag = fragmentPosition - lightPosition;
    float currDist = length(lightToFrag);
    float currentDepth = currDist / far_plane;

    if (!samplingEnabled) {
        red = texture(depthMap, vec4(lightToFrag, currentDepth));
        return;
    }

    vec3 ltf = normalize(lightToFrag);
    vec2 horizontal = computeHorizontalFromDirection(ltf);

    switch (samplingMethod) {
    case 0: red = pcf(horizontal, currentDepth); break;
    case 1: red = pds(horizontal, currentDepth); break;
    }
}

vec2 computeHorizontalFromDirection(in vec3 direction) {
    float AltitudeInRadians = asin(direction.z);
    float altitude = degrees(AltitudeInRadians);
    float azimuth;
    float cosineAltitude = cos(AltitudeInRadians);
    float sineAzimuth = direction.y / cosineAltitude;
    float cosineAzimuth = direction.x / cosineAltitude;
    if (cosineAzimuth >= 0) { // [-pi/2, pi/2]
        azimuth = degrees(asin(sineAzimuth));
    }
    else if (sineAzimuth >= 0) { // [pi/2, pi] 
        azimuth = degrees(acos(cosineAzimuth));
    }
    else { // [-pi, -pi/2]
        azimuth = degrees(-asin(sineAzimuth)) - 180.0;
    }
    return vec2(azimuth, altitude);
}

vec3 computeDirectionFromHorizontal(in vec2 horizontal) {
    float AltitudeInRadians = radians(horizontal.y);
    float AzimuthInRadians = radians(horizontal.x);
    return vec3(
        cos(AltitudeInRadians) * cos(AzimuthInRadians),
        cos(AltitudeInRadians) * sin(AzimuthInRadians),
        sin(AltitudeInRadians)
    );
}

float pcf (vec2 horizontal, float currentDepth) {
    float shadow = 0.0;
    ivec2 tSize = textureSize(depthMap, 0);
    float spread = pow(samplingSpread, 1.5) / length(tSize);

    if (filterSize % 2 == 0) { // even
        int halfSize = filterSize / 2;
        for (int i = -halfSize; i < halfSize; i++) {
            for (int j = -halfSize; j < halfSize; j++) {
                vec3 dir = computeDirectionFromHorizontal(horizontal + (vec2(i, j) + 0.5) * spread);
                shadow += texture(depthMap, vec4(dir, currentDepth));
            }
        }
    } else { // odd
        int projection = filterSize / 2;
        for (int i = -projection; i <= projection; i++) {
            for (int j = -projection; j <= projection; j++) {
                vec3 dir = computeDirectionFromHorizontal(horizontal + vec2(i, j) * spread);
                shadow += texture(depthMap, vec4(dir, currentDepth));
            }
        }
    }
    return shadow / (filterSize * filterSize);
}

float pds (vec2 horizontal, float currentDepth) {
    float shadow = 0.0;
    ivec2 tSize = textureSize(depthMap, 0);
    float spread = pow(samplingSpread, 1.7) / sqrt(tSize.x * tSize.y);

    const int bailing = 4;
    for (int i = 0; i < poissonDisk.length(); i++) {
        vec2 offset;
        if (samplingStratified) {
            int index = int(poissonDiskLarge.length() * random(vec4(gl_FragCoord.xyy, i)));
            offset = poissonDiskLarge[index];
        } else offset = poissonDisk[i];

        vec3 dir = computeDirectionFromHorizontal(horizontal + offset * spread);
        shadow += texture(depthMap, vec4(dir, currentDepth));
        if (samplingStratified && i == bailing - 1 && (shadow == 0.0 || shadow == float(bailing))) { 
            shadow *= poissonDisk.length() / bailing; break; 
        }
    }

    return shadow / poissonDisk.length();
}