#ifndef PARTICLE_H
#define PARTICLE_H
#include "ofVec3f.h"
#include "ofMain.h"

class Particle {
public:
    Particle(int ID, int type_, ofVec3f loc, float mass, ofVec3f vel);
    void update();
    void draw(int dim);
    void draw2D();
    void draw3D();
    void bounce();
    
    // steering behaviors
    void addForce(ofVec3f oForce);
    void subtractForce(ofVec3f oForce);
    void seek(ofVec3f oTarget); // go to a point
    void flee(ofVec3f oTarget); // flee from this target
    void arrive(ofVec3f oTarget); // arrive at this point.
    void pursue(Particle* pParticle);
    
    //void evade(Boid* pBoid);
    
    void updateVertexArrays(int i, float* posBuffer, float* colBuffer);
    
    // getter|setters
    ofVec3f getPosition();
    ofVec3f getVelocity();
    
    void setPosition(ofVec3f oPos);
    void setMaxVelocity(float fMax);
    float getMaxVelocity();
    void setVelocity(ofVec3f oVel);
    void setMaxForce(float fMaxForce);
    void setMass(float fMass);
    void setArrive(float fThresh);
    
    void collide();
    
    ofColor c;
    float max_force;
    float max_vel;
    float mass, radius, dragF;
    float arrive_dist, largestMass;
    int ID, age;
    int type;  // we're going to utilise this to create positive and negative charges
    ofVec3f force;
    ofVec3f pos, origin;
    ofVec3f acc;
    ofVec3f vel, vec;
    bool isDead;
    

    
private:
    ofImage img;
    
    
};

#endif
