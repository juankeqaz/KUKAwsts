import QtQuick 2.0
import Qt3D.Core 2.0
import Qt3D.Render 2.9
import Qt3D.Input 2.0
import Qt3D.Extras 2.9

Entity {
    id:lights
    Entity{
        components: [
        PointLight{
                color: "red"
                intensity: 0.3
                constantAttenuation: 1.0
                linearAttenuation: 0.0
                quadraticAttenuation: 0.0025

            },
            Transform{
                translation: Qt.vector3d(0.0,5.0,0.0)
            }


        ]
    }
    Entity
    {
        components: [
        DirectionalLight{
                worldDirection: Qt.vector3d(0.3,0.3,0.0).normalized();
                color: "#fbf9ce"
                intensity: 0.3
            }

        ]
    }
    Entity
    {
        components: [
        DirectionalLight{
                worldDirection: Qt.vector3d(0.3,0.3,0.0).normalized();
                color: "#9cdaef"
                intensity: 0.15
            }

        ]
    }
    Entity
    {
        components: [
            SpotLight{
              localDirection: Qt.vector3d(0.0,-1.0,0.0)
              color: "white"
              intensity: 0.5
            },
            Transform{
                id:spotLighttransform
                translation: Qt.vector3d(0.0,5.0,0.0);
                rotation: -60.0

            }

        ]
    }
}
