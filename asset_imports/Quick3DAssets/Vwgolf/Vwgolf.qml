import QtQuick
import QtQuick3D

Node {
    // Materials
    // end of Materials

    Node {
        id: vwgolf_obj
        Model {
            id: back_light_glass
            source: "meshes/seat_front.mesh"
            materials: interiors
        }
        Model {
            id: grile_bamper
            source: "meshes/grile_bamper.mesh"
            materials: [
                material__1380_material
            ]
        }
        Model {
            id: logobase
            source: "meshes/logobase.mesh"
            materials: [
                material__1379_material
            ]
        }
        Model {
            id: front_logo
            source: "meshes/front_logo.mesh"
            materials: [
                material__1378_material
            ]
        }
        Model {
            id: grille__
            source: "meshes/grille__.mesh"
            materials: [
                material__1379_material,
                material__1378_material,
                material__1380_material
            ]
        }
        Model {
            id: antifogg1
            source: "meshes/antifogg1.mesh"
            materials: [
                material__1379_material,
                material__1377_material,
                material__1384_material
            ]
        }
        Model {
            id: grille_front
            source: "meshes/grille_front.mesh"
            materials: [
                material__1379_material
            ]
        }
        Model {
            id: car_body
            source: "meshes/grille_front.mesh"
            pickable: false
            receivesReflections: false
            materials: [
                material__1366_material,
                material__1373_material,
                material__1376_material,
                material__1375_material,
                material__1372_material,
                material__89_material
            ]
        }
        Model {
            id: logo_back
            source: "meshes/logo_back.mesh"
            materials: [
                material__1393_material
            ]
        }
        Model {
            id: tire
            source: "meshes/tire.mesh"
            materials: [
                material__1387_material
            ]
        }
        Model {
            id: brake
            source: "meshes/brake.mesh"
            materials: [
                material__1385_material
            ]
        }
        Model {
            id: bOLTS
            source: "meshes/bOLTS.mesh"
            materials: [
                material__1385_material
            ]
        }
        Model {
            id: object06
            source: "meshes/object06.mesh"
            materials: [
                material__1386_material
            ]
        }
        Model {
            id: bRAKE_DISC
            source: "meshes/bRAKE_DISC.mesh"
            materials: [
                material__1388_material
            ]
        }
        Model {
            id: tube08
            source: "meshes/tube08.mesh"
            materials: [
                material__1385_material
            ]
        }
        Model {
            id: antena
            source: "meshes/antena.mesh"
            materials: [
                material__1372_material
            ]
        }
        Model {
            id: antifogg02
            source: "meshes/antifogg02.mesh"
            materials: [
                material__1379_material,
                material__1377_material,
                material__1384_material
            ]
        }
        Model {
            id: front_number
            source: "meshes/front_number.mesh"
            materials: platemat
        }
        Model {
            id: wipers
            source: "meshes/wipers.mesh"
            materials: [
                material__1382_material
            ]
        }
        Model {
            id: light_bmp_bc
            source: "meshes/light_bmp_bc.mesh"
            materials: [
                material__1391_material
            ]
        }
        Model {
            id: back_wiper
            source: "meshes/back_wiper.mesh"
            materials: [
                material__1389_material
            ]
        }
        Model {
            id: back_number
            source: "meshes/back_number.mesh"
            materials: platemat
        }
        Model {
            id: stop_spoiler
            x: -0
            y: 1.881
            source: "meshes/stop_spoiler.mesh"
            materials: [
                material__1393_material,
                material__1391_material
            ]
        }
        Model {
            id: golf_text
            source: "meshes/golf_text.mesh"
            materials: [
                material__1393_material
            ]
        }
        Model {
            id: mirror_b
            source: "meshes/mirror_b.mesh"
            materials: [
                material__1369_material,
                material__1366_material
            ]
        }
        Model {
            id: miror_a__
            source: "meshes/miror_a__.mesh"
            materials: [
                material__1372_material,
                material__1366_material,
                material__91_material
            ]
        }
        Model {
            id: light_front_right
            source: "meshes/light_front_right.mesh"
            materials: frontLighMat
        }
        Model {
            id: light_front_left
            source: "meshes/light_front_left.mesh"
            materials: frontLighMat
        }
        Model {
            id: tire01
            source: "meshes/tire01.mesh"
            materials: [
                material__1387_material
            ]
        }
        Model {
            id: brake01
            source: "meshes/brake01.mesh"
            materials: [
                material__1385_material
            ]
        }
        Model {
            id: bOLTS01
            source: "meshes/bOLTS01.mesh"
            materials: [
                material__1385_material
            ]
        }
        Model {
            id: object24
            source: "meshes/object24.mesh"
            materials: [
                material__1386_material
            ]
        }
        Model {
            id: bRAKE_DISC01
            source: "meshes/bRAKE_DISC01.mesh"
            materials: [
                material__1388_material
            ]
        }
        Model {
            id: tube09
            source: "meshes/tube09.mesh"
            materials: [
                material__1385_material
            ]
        }
        Model {
            id: interior
            source: "meshes/back_light_glass.mesh"
            materials: backLightMaterial
        }
        Model {
            id: doors_panel
            source: "meshes/doors_panel.mesh"
            materials: interiors
        }
        Model {
            id: seats_back
            source: "meshes/seats_back.mesh"
            materials: interiors
        }
        Model {
            id: seat_front
            source: "meshes/seat_front.mesh"
            materials: [
                material__1404_material,
                material__1400_material
            ]
        }
        Model {
            id: seat_front01
            source: "meshes/seat_front01.mesh"
            materials: interiors
        }
        Model {
            id: salon_mirror
            source: "meshes/salon_mirror.mesh"
            materials: [
                material__1405_material,
                material__1399_material
            ]
        }
        Model {
            id: back_light
            source: "meshes/back_light.mesh"
            materials: [
                material__1397_material,
                material__1393_material,
                material__1391_material,
                material__1389_material,
                material__1392_material,
                material__1390_material
            ]
        }
        Model {
            id: steering_wheel
            source: "meshes/steering_wheel.mesh"
            materials: interiors
        }
        Model {
            id: st__wheel_
            source: "meshes/st__wheel_.mesh"
            materials: [
                material__1399_material
            ]
        }
        Model {
            id: dishboard
            source: "meshes/car_body.mesh"
            materials: [carBodyMat,glass]
        }
        Model {
            id: floor_gearbox_panel
            source: "meshes/floor_gearbox_panel.mesh"
            materials: interiors
        }
        Model {
            id: gearbox
            source: "meshes/seat_front.mesh"
            materials: interiors
        }
        Model {
            id: instrument_panel
            source: "meshes/instrument_panel.mesh"
            materials: [
                material__1406_material
            ]
        }
        Model {
            id: logo_st_wheel
            source: "meshes/logo_st_wheel.mesh"
            materials: [
                material__1400_material
            ]
        }
        Model {
            id: brake02
            source: "meshes/brake02.mesh"
            materials: [
                material__1385_material
            ]
        }
        Model {
            id: bOLTS02
            source: "meshes/bOLTS02.mesh"
            materials: [
                material__1385_material
            ]
        }
        Model {
            id: object25
            source: "meshes/object25.mesh"
            materials: [
                material__1386_material
            ]
        }
        Model {
            id: bRAKE_DISC02
            source: "meshes/bRAKE_DISC02.mesh"
            materials: [
                material__1388_material
            ]
        }
        Model {
            id: tube10
            source: "meshes/tube10.mesh"
            materials: [
                material__1385_material
            ]
        }
        Model {
            id: tire02
            source: "meshes/tire02.mesh"
            materials: [
                material__1387_material
            ]
        }
        Model {
            id: tire03
            source: "meshes/tire03.mesh"
            materials: [
                material__1387_material
            ]
        }
        Model {
            id: brake03
            source: "meshes/brake03.mesh"
            materials: [
                material__1385_material
            ]
        }
        Model {
            id: bOLTS03
            source: "meshes/bOLTS03.mesh"
            materials: [
                material__1385_material
            ]
        }
        Model {
            id: object26
            source: "meshes/object26.mesh"
            materials: [
                material__1386_material
            ]
        }
        Model {
            id: bRAKE_DISC03
            source: "meshes/bRAKE_DISC03.mesh"
            materials: [
                material__1388_material
            ]
        }
        Model {
            id: tube11
            source: "meshes/tube11.mesh"
            materials: [
                material__1385_material
            ]
        }
    }
    
    Node {
        id: __materialLibrary__
        DefaultMaterial {
            id: defaultMaterial_material
            objectName: "defaultMaterial_material"
            diffuseColor: "#ababab"
        }
        
        DefaultMaterial {
            id: interiors
            objectName: "interiors"
            diffuseColor: "#313131"
        }
        
        DefaultMaterial {
            id: material__1389_material
            objectName: "material__1389_material"
            diffuseColor: "#000000"
        }
        
        DefaultMaterial {
            id: material__1390_material
            blendMode: DefaultMaterial.Multiply
            objectName: "material__1390_material"
            diffuseColor: "#363d3f"
        }
        
        DefaultMaterial {
            id: material__1380_material
            objectName: "material__1380_material"
            diffuseColor: "#1b1b1b"
        }
        
        DefaultMaterial {
            id: material__1379_material
            objectName: "material__1379_material"
            diffuseColor: "#1e1e1e"
        }
        
        DefaultMaterial {
            id: material__1378_material
            objectName: "material__1378_material"
            diffuseColor: "#9e9e9e"
        }
        
        DefaultMaterial {
            id: material__1377_material
            objectName: "material__1377_material"
            diffuseColor: "#717577"
        }
        
        DefaultMaterial {
            id: material__1384_material
            specularTint: "#46a8dcff"
            objectName: "material__1384_material"
            diffuseColor: "#7acfbeac"
        }
        
        DefaultMaterial {
            id: material__1366_material
            specularTint: "#1a425d"
            objectName: "material__1366_material"
            diffuseColor: "#5e6163"
        }
        
        DefaultMaterial {
            id: material__1373_material
            objectName: "material__1373_material"
            diffuseColor: "#999999"
        }
        
        DefaultMaterial {
            id: material__1376_material
            objectName: "material__1376_material"
            diffuseColor: "#999999"
        }
        
        DefaultMaterial {
            id: material__1375_material
            objectName: "material__1375_material"
            diffuseColor: "#999999"
        }
        
        DefaultMaterial {
            id: material__1372_material
            objectName: "material__1372_material"
            diffuseColor: "#333333"
        }
        
        DefaultMaterial {
            id: material__89_material
            objectName: "material__89_material"
            diffuseColor: "#999999"
        }
        
        DefaultMaterial {
            id: material__1393_material
            objectName: "material__1393_material"
            diffuseColor: "#363636"
        }
        
        DefaultMaterial {
            id: material__1387_material
            objectName: "material__1387_material"
            diffuseColor: "#343739"
        }
        
        DefaultMaterial {
            id: material__1385_material
            objectName: "material__1385_material"
            diffuseColor: "#999999"
        }
        
        DefaultMaterial {
            id: material__1386_material
            objectName: "material__1386_material"
            diffuseColor: "#999999"
        }
        
        DefaultMaterial {
            id: material__1388_material
            objectName: "material__1388_material"
            diffuseColor: "#999999"
        }
        
        DefaultMaterial {
            id: material__1382_material
            objectName: "material__1382_material"
            diffuseColor: "#262626"
        }
        
        DefaultMaterial {
            id: material__1391_material
            objectName: "material__1391_material"
            diffuseColor: "#999999"
        }

        DefaultMaterial {
            id: material__1398_material
            objectName: "material__1398_material"
            diffuseColor: "#999999"
        }

        DefaultMaterial {
            id: material__1369_material
            objectName: "material__1369_material"
            diffuseColor: "#999999"
        }

        DefaultMaterial {
            id: material__91_material
            objectName: "material__91_material"
            diffuseColor: "#999999"
        }

        DefaultMaterial {
            id: material__1381_material
            objectName: "material__1381_material"
            diffuseColor: "#858585"
        }

        DefaultMaterial {
            id: material__1383_material
            objectName: "material__1383_material"
            diffuseColor: "#999999"
        }

        DefaultMaterial {
            id: material__1399_material
            objectName: "material__1399_material"
            diffuseColor: "#999999"
        }

        DefaultMaterial {
            id: material__1403_material
            objectName: "material__1403_material"
            diffuseColor: "#999999"
        }

        DefaultMaterial {
            id: material__1404_material
            objectName: "material__1404_material"
            diffuseColor: "#999999"
        }

        DefaultMaterial {
            id: material__1400_material
            objectName: "material__1400_material"
            diffuseColor: "#999999"
        }

        DefaultMaterial {
            id: material__1405_material
            objectName: "material__1405_material"
            diffuseColor: "#999999"
        }

        DefaultMaterial {
            id: material__1397_material
            objectName: "material__1397_material"
            diffuseColor: "#999999"
        }

        DefaultMaterial {
            id: material__1392_material
            objectName: "material__1392_material"
            diffuseColor: "#999999"
        }

        DefaultMaterial {
            id: material__1402_material
            objectName: "material__1402_material"
            diffuseColor: "#999999"
        }

        DefaultMaterial {
            id: material__1406_material
            objectName: "material__1406_material"
            diffuseColor: "#999999"
        }

        DefaultMaterial {
            id: backLightMaterial
            specularTint: "#ffffff"
            blendMode: DefaultMaterial.SourceOver
            diffuseColor: "#7a0000"
            objectName: "backLightMaterial"
        }

        DefaultMaterial {
            id: platemat
            diffuseColor: "#4c4c4c"
            objectName: "platemat"
        }
        
        DefaultMaterial {
            id: carBodyMat
            diffuseColor: "#7d7d7d"
            objectName: "CarBodyMat"
        }

        DefaultMaterial {
            id: frontLighMat
            diffuseColor: "#555b5b"
            blendMode: DefaultMaterial.SourceOver
            objectName: "FrontLighMat"
        }

        DefaultMaterial {
            id: glass
            diffuseColor: "#3b4040"
            vertexColorsEnabled: false
            blendMode: DefaultMaterial.Multiply
            objectName: "Glass"
        }
    }
}
