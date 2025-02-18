pluginManagement {
    repositories {
        google {
            content {
                includeGroupByRegex("com\\.android.*")
                includeGroupByRegex("com\\.google.*")
                includeGroupByRegex("androidx.*")
            }
        }
        maven {
            url = uri("https://nexus.upuphone.com/repository/xr-snapshots/")
        }
        maven {
            url = uri("https://nexus.upuphone.com/repository/xr-releases/")
        }
        mavenCentral()
        gradlePluginPortal()
    }
}
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        maven {
            url = uri("https://nexus.upuphone.com/repository/xr-snapshots/")
        }
        maven {
            url = uri("https://nexus.upuphone.com/repository/xr-releases/")
        }
        mavenCentral()
    }
}

rootProject.name = "erpc"
include(":erpclib")
include(":helloclient")
include(":helloserver")