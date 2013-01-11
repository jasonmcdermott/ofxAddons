#include "Particles.h"

Particles::Particles(int dim_) {
    // perhaps you might want to adjust these?
    minBounds.set(0,0,0);
    maxBounds.set(ofGetWidth(),ofGetHeight(),800);
    dim = dim_;
    personalSpace = 25;
    visualField = 100;
    mostMassiveParticleID = 0;
}

void Particles::addParticles(int num_, int type_) {
    for (int i=0;i<num_;i++) {
        ofVec3f randomLoc;
        randomLoc.set(ofRandom(minBounds.x,maxBounds.x),ofRandom(minBounds.y, maxBounds.y),ofRandom(minBounds.z,maxBounds.z));
        ofVec3f randomVel;
        randomVel.set(ofRandom(-100,100),ofRandom(-100,100),ofRandom(-100,100));
        float randomMass = ofRandom(0.2,5);
        addParticles(type_,randomLoc,randomMass,randomVel);
    }
}

void Particles::addParticles(int num_, int type_, float mass_){
    for (int i=0;i<num_;i++) {
        ofVec3f randomLoc;
        randomLoc.set(ofRandom(minBounds.x,maxBounds.x),ofRandom(minBounds.y, maxBounds.y),ofRandom(minBounds.z,maxBounds.z));
        ofVec3f randomVel;
        randomVel.set(ofRandom(-100,100),ofRandom(-100,100),ofRandom(-100,100));
        addParticles(type_,randomLoc,mass_,randomVel);
    }
}

void Particles::addParticles(int num_, int type_, ofVec3f loc_){
	for (int i = 0; i < num_; ++i) {
        float randomMass = ofRandom(0.2,5);
        ofVec3f randomVel;
        randomVel.set(ofRandom(-100,100),ofRandom(-100,100),ofRandom(-100,100));
        addParticles(type_,loc_,randomMass,randomVel);
	}
}

void Particles::addParticles(int num_, int type_, ofVec3f loc_, float mass_, ofVec3f vel_){
    for (int i=0;i<num_;i++) {
        addParticles(type_,loc_,mass_,vel_);
    }
}

void Particles::addParticles(int type_, ofVec3f loc_, float mass_,ofVec3f vel_){
    particles.push_back(new Particle(particleCount,type_,loc_,mass_,vel_));
    particleCount++;
}

void Particles::update() {
	for(int i = 0; i < particles.size(); ++i) {
        particles[i]->update();
	}
}

void Particles::draw() {
	for(int i = 0; i < particles.size(); ++i) {
		particles[i]->draw(dim);
	}
}

void Particles::drawGLPoints() {
	glEnable(GL_POINT_SIZE);
    glPointSize(10);
	glBegin(GL_POINTS);
    for(int i = 0; i < particles.size(); ++i) {
        glVertex3f(particles[i]->pos.x, particles[i]->pos.y, particles[i]->pos.z);
	}
	glEnd();
	glDisable(GL_POINT_SIZE);
}

void Particles::drawVertexArrays() {
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
////    glEnable(GL_LINE_SMOOTH);
//
////    for(int i = 0; i < particles.size(); ++i) {
////		particles[i]->updateVertexArrays(i, posArray, colArray);
////	}
//    glEnableClientState(GL_VERTEX_ARRAY);
//    
//    GLfloat quadVertices[] = { -100.0f, 100.0f, 0.0f,
//        100.0f, 100.0f, 0.0f,
//        100.0f,-100.0f, 0.0f,
//        -100.0f,-100.0f, 0.0f
//    };
//    
//
//    
//
//
////    glVertexPointer(2, GL_FLOAT, 0, posArray);
//
//    glVertexPointer(3, GL_FLOAT, 0, quadVertices);
////    glEnableClientState(GL_COLOR_ARRAY);
////    glColorPointer(3, GL_FLOAT, 0, colArray);
//    
//    glDrawArrays(GL_QUADS, 0, 4);
////    glDrawArrays(GL_LINES, 0, particleCount * 2);
//    
//    glDisableClientState(GL_VERTEX_ARRAY);
////    glDisableClientState(GL_COLOR_ARRAY);
//    glDisable(GL_BLEND);
}

// function trying to keep the particles within our given bounds.  not always successful!
void Particles::fleeBounds() {
    ofVec3f fleePos;
    for(int i = 0; i < particles.size(); ++i) {
        ofVec3f tempPos = particles[i]->getPosition();

        if (tempPos.x < minBounds.x + visualField) {
            fleePos.set(minBounds.x,tempPos.y,tempPos.z);
            particles[i]->flee(fleePos);
        }
        if (tempPos.x > maxBounds.x-visualField) {
            fleePos.set(maxBounds.x,tempPos.y,tempPos.z);
            particles[i]->flee(fleePos);
        }
        if (tempPos.y < minBounds.y+visualField) {
            fleePos.set(tempPos.x,minBounds.y,tempPos.z);
            particles[i]->flee(fleePos);
        }
        if (tempPos.y > maxBounds.y-visualField){
            fleePos.set(tempPos.x,maxBounds.y,tempPos.z);
            particles[i]->flee(fleePos);
        }
        if (tempPos.z > minBounds.z+visualField){
            fleePos.set(tempPos.x,tempPos.y,minBounds.z);
            particles[i]->flee(fleePos);
        }
        if (tempPos.z > maxBounds.z-visualField){
            fleePos.set(tempPos.x,tempPos.y,maxBounds.z);
            particles[i]->flee(fleePos);
        }
    }
}

void Particles::transportBounds() {
	for(int i = 0; i < particles.size(); ++i) {
		if (particles[i]->pos.x > maxBounds.x) {
            particles[i]->pos.x = minBounds.x;
        }
        if (particles[i]->pos.x < minBounds.x) {
            particles[i]->pos.x = maxBounds.x;
        }
        if (particles[i]->pos.y > maxBounds.y) {
            particles[i]->pos.y = minBounds.y;
        }
        if (particles[i]->pos.y < minBounds.y) {
            particles[i]->pos.y = maxBounds.y;
        }
        if (particles[i]->pos.z > maxBounds.z) {
            particles[i]->pos.z = minBounds.z;
        }
        if (particles[i]->pos.z < -minBounds.z) {
            particles[i]->pos.z = maxBounds.z;
        }
	}
}

void Particles::bounce() {
	for(int i = 0; i < particles.size(); ++i) {
		if (particles[i]->pos.x > maxBounds.x) {
            particles[i]->pos.x = maxBounds.x;
            particles[i]->vel.x *= -1;
        }
        if (particles[i]->pos.x < minBounds.x) {
            particles[i]->pos.x = minBounds.x;
            particles[i]->vel.x *= -1;
        }
        if (particles[i]->pos.y > maxBounds.y) {
            particles[i]->pos.y = maxBounds.y;
            particles[i]->vel.y *= -1;
        }
        if (particles[i]->pos.y < minBounds.y) {
            particles[i]->pos.y = minBounds.y;
            particles[i]->vel.y *= -1;
        }
        if (particles[i]->pos.z > maxBounds.z) {
            particles[i]->pos.z = maxBounds.z;
            particles[i]->vel.z *= -1;
        }
        if (particles[i]->pos.z < minBounds.z) {
            particles[i]->pos.z = minBounds.z;
            particles[i]->vel.z *= -1;
        }
	}
}

void Particles::seekParent(ofVec3f oMainTarget) {
	particles[0]->seek(oMainTarget);
	for(int i = 1; i < particles.size(); ++i) {
		particles[i]->seek(particles[i-1]->getPosition());
	}
}

void Particles::orbit() {
    for (int i=0;i<particles.size();i++) {
        for (int j=0;j<particles.size();j++) {
            if (j == i) continue;
            if (particles[i]->type == 0) {
                particles[i]->seek(particles[j]->pos);
            }
        }
    }
}

void Particles::collide() {
    for (int i = 0; i < particles.size(); ++i) {
		for(int j = 0; j < particles.size(); ++j) {
            if (j == i) continue;
            if (particles[i]->isDead || particles[j]-> isDead) continue;
            ofVec3f aPos = particles[i]->pos;
            ofVec3f bPos = particles[j]->pos;
            float dist = aPos.distance(bPos);
            if (dist < (particles[i]->radius + particles[j]->radius)) {
                
                if (particles[i]->mass > particles[j]->mass) {
                    particles[i]->mass += particles[j]->mass/particles[i]->mass;
                    particles[j]->collide();
                } else {
                    particles[j]->mass += particles[i]->mass/particles[j]->mass;
                    particles[i]->collide();
                }
            }
        }
    }
}

void Particles::repulse() {
    tallyMass();
	for (int i = 0; i < particles.size(); ++i) {
		for(int j = 0; j < particles.size(); ++j) {
            if (j == i) continue;
            if (particles[i]->type == particles[j]->type) {
                ofVec3f aPos = particles[i]->pos;
                ofVec3f bPos = particles[j]->pos;
                ofVec3f dir = aPos-bPos;
                float dist = aPos.distance(bPos);
                if (dist > 0.0) {
                    dir.normalize();
                    float F = 1.0f / dist;
                    float iMass = particles[i]->mass / totalMass;
                    float jMass = particles[j]->mass / totalMass;
                    particles[i]->addForce(dir*(F/iMass));
                    particles[j]->subtractForce(dir*(F/jMass));
                }
            }
        }
    }
}

void Particles::attract() {
    tallyMass();
	for (int i = 0; i < particles.size(); ++i) {
		for(int j = 0; j < particles.size(); ++j) {
            if (j == i) continue;
            if (particles[i]->isDead == false && particles[j]->isDead == false) {
                if (particles[i]->type > particles[j]->type) continue;
                ofVec3f aPos = particles[i]->pos;
                ofVec3f bPos = particles[j]->pos;
                ofVec3f dir = aPos-bPos;
                float dist = aPos.distance(bPos);
                if (dist > 0.0 ) {
                    dir.normalize();
                    float F = 1.0f / dist;
                    float iMass = particles[i]->mass / totalMass;
                    float jMass = particles[j]->mass / totalMass;
                    particles[i]->subtractForce(dir*(F/iMass));
                }
            }
        }
    }
}


void Particles::connectingLines(int col) {
    for (int i = 0; i < particles.size(); ++i) {
        for (int j = 0; j < particles.size(); j++) {
            if (j == i) continue;
            if (particles[i]->isDead == false && particles[j]->isDead == false) {
                ofVec3f aPos = particles[i]->pos;
                ofVec3f bPos = particles[j]->pos;
                ofVec3f dir = aPos-bPos;
                float dist = aPos.distance(bPos);
                if (dist < 10000) {
                    ofSetColor(255-col);
                    ofSetLineWidth(10);
                    ofLine(particles[i]->pos,particles[j]->pos);
                }
            }
        }
    }
}

ofVec3f Particles::getLargestBodyPosition() {
    float bigger = 0;
	for (int i = 0; i < particles.size(); ++i) {
        if (particles[i]->isDead == true) continue;
        if (particles[i]->mass > bigger) {
            bigger = particles[i]->mass;
            mostMassiveParticleID = i;
        }
    }
    
    for (int i = 0; i < particles.size(); ++i) {
        particles[i]->largestMass = bigger;
    }
    ofVec3f loc;
    loc.set(particles[mostMassiveParticleID]->pos);
    return loc;
}

ofVec3f Particles::getLargestParticleVector() {
    ofVec3f loc;
    loc.set(particles[mostMassiveParticleID]->vec);
    return loc;

}

void Particles::flock() {
	
	int insight_count = 0;
	ofVec3f average_vel, average_pos;
	for (int i = 0; i < particles.size(); ++i) {
		average_vel.set(0,0,0);
		average_pos.set(0,0,0);
		insight_count = 0;

		//for(int j = i + 1; j < particles.size(); ++j) {
		for(int j = 0; j < particles.size(); ++j) {
			if (j == i) continue;
            if (particles[i]->type != particles[j]->type) continue;
			float dist = (particles[i]->getPosition() - particles[j]->getPosition()).length();

			// separate
			if (dist <= visualField ) {
				average_vel += particles[j]->getVelocity();
				average_pos += particles[j]->getPosition();
				insight_count++;
				if (dist <= personalSpace) {
					particles[i]->flee(particles[j]->getPosition());
					/*
					ofxVec3f heading = particles[i]->getVelocity().normalize();
					ofxVec3f diff = boids[j]->getPosition()-particles[i]->getPosition();
					float dot_prod = diff.dot(heading);
					if (dot_prod > 0) {
						particles[i]->flee(boids[j]->getPosition());
					}
					*/
				}
			}
		}
		if (insight_count > 0) {
			average_pos /= insight_count;
			average_vel /= insight_count;
			particles[i]->seek(average_pos);
			particles[i]->addForce(average_vel - particles[i]->getVelocity());
		}
	}
}

vector<Particle*> Particles::getParticles() {
	return particles;
}

void Particles::setMaxVelocity(float fMax) {
	for (int i = 0; i < particles.size(); ++i) {
		particles[i]->setMaxVelocity(fMax);
	}
}

void Particles::setMaxForce(float fMax) {
    for (int i = 0; i < particles.size(); ++i) {
		particles[i]->setMaxForce(fMax);
	}
}

void Particles::arrive(ofVec3f oTarget) {
	for (int i = 0; i < particles.size(); ++i) {
		particles[i]->arrive(oTarget);
	}
}

void Particles::flee(ofVec3f oTarget) {
	for (int i = 0; i < particles.size(); ++i) {
		particles[i]->flee(oTarget);
	}
}

void Particles::setMass(float fMass) {
    for (int i = 0; i < particles.size(); ++i) {
		particles[i]->setMass(fMass);
	}
}

void Particles::setMass(int type_, float fMass) {
    for (int i = 0; i < particles.size(); ++i) {
        if (particles[i]->type == type_) {
            particles[i]->setMass(fMass);
        }
	}
}

void Particles::setArriveThreshold(float fThresh){
    for (int i = 0; i < particles.size(); ++i) {
		particles[i]->setArrive(fThresh);
	}
}

void Particles::explode() {
	float max_vel = particles[0]->getMaxVelocity();
	for (int i = 0; i < particles.size(); ++i) {
		particles[i]->setVelocity(ofVec3f(ofRandom(-max_vel,max_vel),ofRandom(-max_vel,max_vel),ofRandom(0,-max_vel)));
	}
}

void Particles::tallyMass() {
    for(int i = 0; i < particles.size(); ++i) {
        totalMass += particles[i]->mass;
    }
}

void Particles::returnToOrigin() {
    setDefaults();
    for (int i = 0; i < particles.size(); ++i) {
        particles[i]->isDead = false;
		particles[i]->pos.set(particles[i]->origin.x,particles[i]->origin.y,particles[i]->origin.z);
	}
}

void Particles::startWithRandomVelocity() {
    ofVec3f randVel;
    randVel.set(ofRandom(minBounds.x,maxBounds.x),ofRandom(minBounds.y, maxBounds.y),ofRandom(minBounds.z,maxBounds.z));
    for (int i = 0; i < particles.size(); ++i) {
        particles[i]->addForce(randVel);
    }
}

void Particles::setDefaults() {
    // Default values, so you don't need to set them in the main class.
    setMaxVelocity(2);
    setMaxForce(2.0f);
    personalSpace = 25;
    visualField = 100;
    setArriveThreshold(140.0);
//    setVelocity();
}


int Particles::countLiveParticles() {
    int count = 0;
	for(int i = 0; i < particles.size(); ++i) {
        if (particles[i]->isDead == false) {
            count++;
        }
	}
    return count;
}