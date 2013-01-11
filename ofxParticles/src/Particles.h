#ifndef BOIDS_H
#define BOIDS_H

#include <vector>
#include "Particle.h"
#include "ofMain.h"
#include "ofVec3f.h"

/**
 * Aggregator for our boids.
 */



class Particles {
	public:
		Particles(int dim);
		void update();
        void draw();
        void drawVertexArrays();
        void drawGLPoints();
//        void addParticles(int iNumParticles, int dim, int type);

        void addParticles(int num, int type);
        void addParticles(int num, int type, float mass);
        void addParticles(int num, int type, ofVec3f loc);    
        void addParticles(int num, int type, ofVec3f loc, float mass, ofVec3f vel);
        void addParticles(int type, ofVec3f loc, float mass, ofVec3f vel);

		void bounce();
		void seekParent(ofVec3f oMainTarget);
		void flock();
        void orbit();
        void repulse();
        void attract();
        void collide();
		vector<Particle*> getParticles();
    
    
		void setMaxVelocity(float fMaxVel);
        void setMaxForce(float fMaxForce);
        void setMass(float fMass);
        void setMass(int type, float fMass);
        void setArriveThreshold(float fThresh);
        void tallyMass();
        void setDefaults();
        void setVelocity();
        void startWithRandomVelocity();
        void arrive(ofVec3f oTarget);
        void flee(ofVec3f oTarget);
        void explode(); // apply random velocity (impuls);
        void fleeBounds();
        void transportBounds();
        void connectingLines(int col);
    
        ofVec3f getLargestBodyPosition();
        ofVec3f getLargestParticleVector();
        void returnToOrigin();
        int countLiveParticles();
    
    int particleCount, dim, mostMassiveParticleID;
    float personalSpace, visualField,totalMass;
    float minBoundsX,maxBoundsX,minBoundsY,maxBoundsY,minBoundsZ,maxBoundsZ;
    ofVec3f minBounds, maxBounds;
    ofVec3f rand2D, rand3D, loc;
    vector<Particle*> particles;
    
    float posArray[1000000 * 2 * 2];
	float colArray[1000000 * 3 * 2];

    
private:
		int num_Particles;
		

};

#endif
