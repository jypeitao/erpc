plugins {
    alias(libs.plugins.android.library)
    alias(libs.plugins.kotlin.android)
}

android {
    namespace = "com.dreamsmart.erpclib"
    compileSdk = 35

    defaultConfig {
        minSdk = 29

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        consumerProguardFiles("consumer-rules.pro")
        externalNativeBuild {
            cmake {
                cppFlags("")
                arguments(
                    "-DUSE_KCONFIG=OFF",
                    "-DCONFIG_ERPC_GENERATOR=n",
                    "-DCONFIG_REQUIRE_ERPCGEN=y",
                    "-DCONFIG_ERPC_EXAMPLES=y",
                    "-DCONFIG_ERPC_HELLO_WORLD=y",
                    "-DCONFIG_ERPC_HELLO_WORLD.c=n",
                    "-DCONFIG_ERPC_HELLO_WORLD.cpp=y",
                    "-DCONFIG_ERPC_LIB.cpp=n"
                )
            }
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }
    externalNativeBuild {
        cmake {
            path("../../CMakeLists.txt")
            version = "3.22.1"
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }
    kotlinOptions {
        jvmTarget = "11"
    }
}

dependencies {

    implementation(libs.androidx.core.ktx)
    implementation(libs.androidx.appcompat)
    implementation(libs.material)
    testImplementation(libs.junit)
    androidTestImplementation(libs.androidx.junit)
    androidTestImplementation(libs.androidx.espresso.core)
}