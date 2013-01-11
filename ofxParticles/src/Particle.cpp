#include "Particle.h"

Particle::Particle(int ID_, int type_, ofVec3f loc_, float mass_, ofVec3f vel_) {
    ID = ID_;
    type = type_;
    origin.set(loc_.x,loc_.y,loc_.z);
    isDead = false;
    age = 0;
    mass = mass_;
    vel.set(vel_.x,vel_.y,vel_.z);
//    max_vel = 2;
//    max_force = 2.0f;
    arrive_dist = 140;
    pos.set(loc_.x,loc_.y,loc_.z);
    c.set(255);
    vec.set(0,0,0);
}

void Particle::update() {
    if (isDead == false) {
        age++;

        if (mass < 1) {
            radius = mass * 20;
        } else {
            radius = mass;
        }

        vel += force;
        pos += vel;
        dragF = ((0.9*0.5) * (vel.lengthSquared()) * 0.47 * (2*radius));
        vel /= dragF;

        ofVec3f rotater;
        rotater.set(mass*2,cos(ofGetElapsedTimef()*1.4) * 30.f,sin(ofGetElapsedTimef()*1.4) * 30.f);
        vec = pos+rotater;
    }
}


void Particle::draw(int dim) {
    if (isDead == false) {
        if (dim == 3) {
            draw3D();
        } else if (dim == 2) {
            draw2D();
        }
    }
}

void Particle::draw3D() {
    ofPushMatrix();
    ofTranslate(pos.x, pos.y, pos.z);
    ofPopMatrix();
    ofSetColor(c);
    ofSphere(pos.x, pos.y, pos.z, radius);
}

void Particle::draw2D() {
	ofEnableSmoothing();
    ofPushMatrix();
    ofTranslate(pos.x, pos.y, pos.z);
    ofPopMatrix();
	ofSetColor(c);
	ofSetLineWidth(1);

	float arrow_len = 20;
	ofVec3f arrow_end = pos +(vel.normalized() * arrow_len);

//	ofLine(pos.x, pos.y, arrow_end.x, arrow_end.y);
//	ofCircle(arrow_end.x, arrow_end.y, 3);
    
    ofCircle(pos.x, pos.y, radius);

	ofDisableSmoothing();
}



void Particle::updateVertexArrays(int i, float* posBuffer, float* colBuffer){
	int vi = i * 4;
	posBuffer[vi++] = pos.x - vel.x;
	posBuffer[vi++] = pos.y - vel.y;
	posBuffer[vi++] = pos.x;
	posBuffer[vi++] = pos.y;
	int ci = i * 6;
}

void Particle::seek(ofVec3f oTarget) {
	ofVec3f desired_vel = oTarget - pos;
    desired_vel.scale(max_vel);
	desired_vel -= vel;
	addForce(desired_vel);
}

void Particle::flee(ofVec3f oTarget) {
	ofVec3f desired_vel = oTarget - pos;
	desired_vel.scale(max_vel);
	desired_vel -= vel;
	subtractForce(desired_vel);
}

void Particle::arrive(ofVec3f oTarget) {
	ofVec3f desired_vel = oTarget - pos;
	float dist = desired_vel.length();
	desired_vel.normalize();
    
	if (dist <= arrive_dist) {
		desired_vel *= ((max_vel * dist / arrive_dist));
		ofSetColor(0);
	}
	else {
		desired_vel *= max_vel;
	}
	if (dist > 10) {
		desired_vel -= vel;
		addForce(desired_vel);
	}
	else{
		force.set(0,0,0);
		vel.set(0,0,0);
	}
}

void Particle::pursue(Particle* pParticle) {
	float dist = (pParticle->getPosition() - getPosition()).length();
	float ahead_time = dist / max_vel;
	ofVec3f predicted_target = pParticle->getPosition() + pParticle->getVelocity()*ahead_time;
	seek(predicted_target);
}

void Particle::addForce(ofVec3f oForce) {
	force += oForce;
}

void Particle::subtractForce(ofVec3f oForce) {
	force -= oForce;
}

ofVec3f Particle::getPosition() {
	return pos;
}

ofVec3f Particle::getVelocity() {
	return vel;
}

void Particle::setPosition(ofVec3f oPos) {
	pos = oPos;
}

void Particle::setMaxVelocity(float fMax) {
	max_vel = fMax;
}

void Particle::setMass(float fMass) {
	mass = fMass;
}

float Particle::getMaxVelocity() {
	return max_vel;
}

void Particle::setMaxForce(float fMaxForce){
    max_force = fMaxForce;
}

void Particle::setVelocity(ofVec3f oVel) {
	vel = oVel;
}

void Particle::setArrive(float fThresh) {
    arrive_dist = fThresh;
}

void Particle::collide(){
    isDead = true;
}

