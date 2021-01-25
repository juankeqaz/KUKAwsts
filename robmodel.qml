import QtQuick 2.1
import Qt3D.Core 2.0
import Qt3D.Render 2.9
import Qt3D.Input 2.0
import Qt3D.Extras 2.9

Entity {
    id:root
    objectName: "root"
    components: [
        RenderSettings{
            activeFrameGraph: ForwardRenderer{
                camera: mainCamera
            }
        },
        InputSettings{}

    ]
    BasicCamera{
        id:mainCamera
        position: Qt.vector3d(-5.18,3.0,6.7)
        viewCenter: Qt.vector3d(6.7,-2.5,-10)
    }
    FirstPersonCameraController{
        camera: mainCamera
    }
    Lights{

    }
    Entity{
        components:[
            PointLight{
                enabled: parent.enabled
                color: "black"
                intensity: 0
            },
            EnvironmentLight{
                enabled: parent.enabled
                WrapMode{

                }
            }

        ]
    }
    Entity{
        id:floor
        components: [
            Mesh{
                source: "qrc:/../kukamodel/toyplane.obj"
            },
            MetalRoughMaterial{
                baseColor: Qt.rgba(0.8,0.0,0.0,1.0)
                metalness: 0.2
                roughness: 0.5
            },
            Transform{
                translation: Qt.vector3d(0.0,0.0,0.0);
                rotationX: 0.0
                rotationY: 0.0
                scale: 1.0
            }

        ]
    }

}
