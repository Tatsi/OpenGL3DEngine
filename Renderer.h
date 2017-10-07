#ifndef RENDERER_H
#define RENDERER_H

//Notes:
//-Renderer currently uses only one directional light which comes from the sky. This is currently hard coded.

class Renderer
{
public:
	static Renderer& get() { return instance; }

	void init(HDC deviceCxtHandle); //Initialize
	//void drawTerrain();
	void drawModels();
	void drawScene();

	float getCameraX();
	float getCameraY();
	float getCameraZ();

	void setCameraX(float new_x);
	void setCameraY(float new_y);
	void setCameraZ(float new_z);
	
	void rotateCameraRight();
	void rotateCameraLeft();
	
private:
	Renderer() { }
	Renderer(const Renderer &);
	Renderer& operator=(const Renderer&);
	HDC deviceContextHandle;
	//Camera parameters
	float camera_x;
	float camera_y;
	float camera_z;
	//Directional light parameters
	GLfloat ambient_light_intensity[3];//Ambient light color/intensity
	//Other light parameters temporarily here
	GLfloat diffuse_light_intensity[3];//Diffuse light color/intensity
	GLfloat specular_light_intensity[3];//Specular light color/intensity

	static Renderer instance;
};

#endif