#include "testApp.h"


vector <ofVec3f> debugVerts;

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    globeImage.loadImage("earth_bw.png");
    oleImage.loadImage("ole.jpg");
    
//dynamicallly create a material. it'll be deleted when the destructor for it's mesh is called
    ofxScene::NormalMaterial* normalMat = new ofxScene::NormalMaterial();

//lights
    pointLights.resize(5);
    lightMeshes.resize(pointLights.size());
    for(int i=0; i<pointLights.size(); i++){
        //set pointLights color and attenuation
        pointLights[i].setColor( ofRandom(1.5), ofRandom(1.5), ofRandom(1.5) );
        pointLights[i].setAttenuation(1,.0001, .0001);//constaint, linear, quadratic
        scene.add( pointLights[i] );
        
        //create some meshes for drawing the point lights
        lightMeshes[i].init(sphereGeometry,
                            new ofxScene::FacingRatioMaterial(ofVec3f(1),                 // color1
                                                              pointLights[i].getColor(),  // color2
                                                              1,                          // alpha
                                                              4 ) );                      // exponent
        lightMeshes[i].scale.set( .1 );
        scene.add( lightMeshes[i] );
    }
    
    spotLight.setCutoff(.9);
    spotLight.setPosition(ofVec3f(0,300,0));
    scene.add( spotLight );
    
    spotLightMesh.init(new ofxScene::ConeGeometry(12,30),
                      new ofxScene::FlatMaterial() );
    spotLightMesh.position = spotLight.getPosition();
    spotLightMesh.wireframe = true;
    spotLightMesh.doubleSided = true;
    spotLightMesh.shader->setUniform( "Diffuse", &spotLightColor );//link "Diffuse" uniform to spotLightColor. it will automatically update everyframe
    scene.add( spotLightMesh );
    
//cube mesh
    cube.init(new ofxScene::CubeGeometry(60,60,60),
              new ofxScene::LambertMaterial(ofVec3f(1),                         //diffuse
                                            ofVec3f(.25,.25,0),                   //ambient
                                            1.,                                 //alpha
                                            &oleImage.getTextureReference()) ); //texture
    cube.position.set(150,150,0);
    scene.add( cube );
    
//sphere mesh
    ofxScene::Mesh* sphere = new ofxScene::Mesh(new ofxScene::SphereGeometry( 50, 20, 10 ),
                                                new ofxScene::PhongMaterial(ofVec3f(1),                             //diffuse
                                                                            ofVec3f(1.1),                           //specular
                                                                            ofVec3f(.1),                            //ambient
                                                                            1,                                      //alpha
                                                                            128,                                    //shineness
                                                                            &globeImage.getTextureReference() ) );  //texture map
    sphere->position.y += 150;
    scene.add( sphere );
    
//parenting
    ofxScene::CubeGeometry* nestedGeometry = new ofxScene::CubeGeometry( 30,30,30 );
    nestedMeshes.resize(100);
    
    for (int i=0; i<nestedMeshes.size(); i++) {
        nestedMeshes[i].init( nestedGeometry, lambertMaterial );
        nestedMeshes[i].position.set( -20, 20, 0 );
        nestedMeshes[i].scale.set( .96 );
        if(i > 0){
            nestedMeshes[i-1].addChild( nestedMeshes[i] );
        }
        
        scene.add( nestedMeshes[i] );
    }
    nestedMeshes[0].position.set( -150, 150, 0 );
    
    
    
//dynamic geometry
    dynamicSphere.init( 50 , 70, 40, false );
    dynamicMesh.init( dynamicSphere, sphere->getShader() );
    scene.add( dynamicMesh );
    
    normalMesh.init( dynamicSphere, normalMat );
    normalMesh.position.x = -150;
    normalMesh.scale.set(.5);
    scene.add( normalMesh );
    
    facingRatioMesh.init(dynamicSphere,
                         new ofxScene::FacingRatioMaterial(ofVec3f(1.5),                        //color1
                                                           ofVec3f(.2,.2,.3),                   //color2
                                                           1.,                                  //alpha
                                                           4,                                   //exponent
                                                           &oleImage.getTextureReference()));   //texture map
    facingRatioMesh.position.x = 150;
    facingRatioMesh.scale.set(.5);
    scene.add( facingRatioMesh );
    
    //copy the original positions and normals to deform later 
    orginalPositions = dynamicSphere.vertices;
    orginalNormals = dynamicSphere.normals;

    
//loading geometry from file
    waltHead.init(new ofxScene::Geometry(),
                  new ofxScene::PhongMaterial(ofVec3f(.4,.4,.75),   //diffuse
                                              ofVec3f(.8),          //specular
                                              ofVec3f(0),           //ambient
                                              1,                    //alpha
                                              16) );                //shininess
    waltHead.geometry->load( "waltHeads/walt_medium.obj" );//http://davidoreilly.com/post/18087489343/disneyhead
    waltHead.position.set( -150, -150, 0 );
    waltHead.scale.set( 1.75 );
    scene.add( waltHead );
    
    waltHead1.init( waltHead.geometry,
                   new ofxScene::PhongMaterial(ofVec3f(.3), //diffuse
                                               ofVec3f(1),  //specular
                                               ofVec3f(0),  //ambient
                                               1,           //alpha
                                               64) );       //shininess
    waltHead1.position.set( 0, -150, 0 );
    waltHead1.scale.set( 1.75 );
    scene.add( waltHead1 );
    
    waltHead2.init( waltHead.geometry, normalMat );
    waltHead2.position.set( 150, -150, 0 );
    waltHead2.scale.set( 1.75 );
    waltHead2.wireframe = true;
    scene.add( waltHead2 );
    
//ground plane
    plane.init(new ofxScene::PlaneGeometry(600,600,20,20),
               dynamicMesh.shader);
    plane.position.set(0.,-250, 0);
    plane.rotate(-90, 1, 0, 0);
    scene.add( plane );
    plane.doubleSided = true;
    plane.geometry->calcTangents();

    //a simple debugging mesh that displays a mesh's normal, tangent, binormal
    normalDisplayMesh.setup( plane );
    scene.add( normalDisplayMesh );
}

//--------------------------------------------------------------
void testApp::update(){
    ofSetWindowTitle( ofToString( ofGetFrameRate()) );
    float elapsedTime = ofGetElapsedTimef();
    

    //move point lights around
    float step = ofDegToRad(360/pointLights.size());
    for(int i=0; i<pointLights.size(); i++){
        
        lightMeshes[i].position.set(sin(float(i)*step+elapsedTime*.25)*100,
                                    sin(float(i)*step+elapsedTime*.5)*200,
                                    cos(float(i)*step+elapsedTime*.25)*100 );
                
        pointLights[i].setPosition( lightMeshes[i].position );
    }
    
    //rotate spotlight and change it's color
    spotLight.setDirection(sin(elapsedTime/2), -abs(cos(elapsedTime/2)), 0);
    spotLightMesh.rotateTo( spotLight.getDirection() * ofVec3f(-1,1,1) );
    spotLightColor.set(1.1) ;//spotLight.getDirection()*.5+.5;
    spotLight.setColor( spotLightColor );
    
    //mesh orientations
    cube.rotate( sin(elapsedTime*.3)*180., cos(elapsedTime*.3)*180., 0 );
    
    waltHead.rotate(mouseX, 0, 1, 0);           //degrees, axisXYZ
    waltHead1.rotate(mouseX + mouseY, 0, 0, 1); //degrees, axisXYZ
    waltHead2.rotate(mouseY, 0, 0);             //dgees x, degrees y, degrees z
    
    //parented meshes
    for (int i=0; i<nestedMeshes.size(); i++) {
        nestedMeshes[i].rotate(10 * sin(elapsedTime+float(i*.1)),   //x axis
                               0,                                   //y axis
                               10 * cos(elapsedTime+float(i*.1)));  //z axis
    }
    
    //dynamic geometry
    ofVec3f pos;
    for(int i=0; i<dynamicSphere.vertices.size(); i++){
        pos = orginalPositions[i] * .02;
        dynamicSphere.vertices[i] = orginalPositions[i] + orginalNormals[i] * ofNoise(pos.x, pos.y, pos.z+elapsedTime) * 50.;
    }
    dynamicSphere.calcVertexNotmals();
    dynamicSphere.update();
    dynamicMesh.rotate( elapsedTime*10., 1,0,0 );
    
    plane.rotate( -90, elapsedTime, 0);
    ofxScene::Geometry* g = plane.geometry;
    float nval = elapsedTime ;
    for(int i=0; i<g->vertices.size(); i++){
        g->vertices[i].z = sin(g->vertices[i].x*.01+nval) * cos(g->vertices[i].y*.01+nval) * 50.;
    }
    g->calcVertexNotmals();
    plane.geometry->calcTangents();
    g->update();
    
    //update mesh that displays a mesh's normal, tangent, binormal
    normalDisplayMesh.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0,0,0);
    scene.draw( camera );
}

//--------------------------------------------------------------
void testApp::exit(){
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case 'F':
            ofToggleFullscreen();
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
    
}