import QtQuick
import QtQuick3D

Node {
    // Materials
    // end of Materials

    Node {
        id: volkslogo_obj
        Model {
            id: uploads_files_122684_volk__1_
            source: "meshes/uploads_files_122684_volk__1_.mesh"
            materials: [
                mat2_material
            ]
        }
        Model {
            id: uploads_files_122684_volk__1__002
            source: "meshes/uploads_files_122684_volk__1__002.mesh"
            materials: [
                mat0_material
            ]
        }
    }

    Node {
        id: __materialLibrary__

        DefaultMaterial {
            id: defaultMaterial_material
            objectName: "defaultMaterial_material"
            diffuseColor: "#ff999999"
        }

        DefaultMaterial {
            id: mat2_material
            objectName: "mat2_material"
            diffuseColor: "#000000"
        }

        DefaultMaterial {
            id: mat0_material
            objectName: "mat0_material"
            diffuseColor: "#adadad"
        }
    }
}
