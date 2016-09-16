#include "stdafx.h"
#include "Renderer.h"

Renderer Renderer::instance = Renderer();

void Renderer::init(HDC deviceCxtHandle)
{
	ambient_light_intensity[0] = 0.6;//Ambient light color/intensity
	ambient_light_intensity[1] = 0.6;
	ambient_light_intensity[2] = 0.6;
	//Init other lights temporarily
	diffuse_light_intensity[0] = 0.7;//Diffuse light color/intensity
	diffuse_light_intensity[1] = 0.7;
	diffuse_light_intensity[2] = 0.7;
	specular_light_intensity[0] = 0.8;//Specular light color/intensity
	specular_light_intensity[1] = 0.8;
	specular_light_intensity[2] = 0.8;
	//End of temporary

	deviceContextHandle = deviceCxtHandle;

	camera_x = 0.0;
	camera_y = 4.0;
	camera_z = 2.0;

	glClearColor(0.0, 0.0, 0.1, 0.0);
    
    glEnable(GL_DEPTH_TEST); //Z-Buffer
    
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void Renderer::drawScene()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
   
	gluLookAt(camera_x, camera_y, camera_z, Player::get().getX(), Player::get().getY(), Player::get().getZ(), 0.0, 1.0, 0.0);

	//drawTerrain();

    drawModels();

	Player::get().draw();
	
	UI::get().draw();
	
    SwapBuffers(deviceContextHandle);

    glFlush();    
}


void Renderer::drawModels()
{
	std::vector<GameObject>& gameObjects = LevelManager::get().getTerrain().getGameObjects();
	//Add other game objects to this list if they should be drawn

	for (std::vector<GameObject>::const_iterator it = gameObjects.begin(); it < gameObjects.end(); it++)
	{
		glPushMatrix();
			Model& model = it->getModel(); 
			
			glUseProgram(model.getShaderProgram()); //Use shader program
			GLfloat scale = it->getScale();//Scaling and transforming must be done the same way to terrain squares which are used for col. detec.
			vec3 position = it->getPosition();
			
			glTranslatef(position.x, position.y, position.z);
			glScalef(scale, scale, scale);

			glsl_parameter_locations glsl_locations = model.getGlslParameterLocations();//Get parameter glsl locations
			//#####################################
			//#Calculate and pass values for GLSL parameters
			//#####################################
			//Pass matrices to shaders
			GLfloat mp[16];
			GLfloat mv[16];
			glGetFloatv(GL_PROJECTION_MATRIX, mp);
			glGetFloatv(GL_MODELVIEW_MATRIX, mv);
			glUniformMatrix4fv(glsl_locations.location_projection_matrix, 1, GL_FALSE, mp);
			glUniformMatrix4fv(glsl_locations.location_modelview_matrix, 1, GL_FALSE, mv);

			//Pass light parameters to shaders
			glUniform3f(glsl_locations.location_viewing_point, Player::get().getX(), Player::get().getY(), Player::get().getZ());
			glUniform3f(glsl_locations.location_ambient_light_intensity, ambient_light_intensity[0], ambient_light_intensity[1], ambient_light_intensity[2]);
			glUniform3f(glsl_locations.location_diffuse_light_intensity, diffuse_light_intensity[0], diffuse_light_intensity[1], diffuse_light_intensity[2]);
			glUniform3f(glsl_locations.location_specular_light_intensity, specular_light_intensity[0], specular_light_intensity[1], specular_light_intensity[2]);
			
			//Get model specific reflection constants and pass the values
			vec3 ambient_RC = model.getAmbientLightReflectionConstant();
			vec3 diffuse_RC = model.getDiffuseLightReflectionConstant();
			vec3 specular_RC = model.getSpecularLightReflectionConstant();
			glUniform3f(glsl_locations.location_ambient_light_reflection_constant, ambient_RC.x, ambient_RC.y, ambient_RC.z);			
			glUniform3f(glsl_locations.location_diffuse_light_reflection_constant, diffuse_RC.x, diffuse_RC.y, diffuse_RC.z);			
			glUniform3f(glsl_locations.location_specular_light_reflection_constant, specular_RC.x, specular_RC.y, specular_RC.z);
			//#######################
        
			glColor3f(1.0, 0.0, 0.0);
    
			glBindVertexArray(model.getVaoID());//Bind vertex array

			glBindBuffer(GL_ARRAY_BUFFER, model.getVertexVboID());//Bind vertex buffer
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.getIndiceIboID());//Bind index buffer
    
			glBindBuffer(GL_ARRAY_BUFFER, model.getTextureCoordinatesVBOID());//Bind texture coords buffer
			

			glEnableClientState(GL_VERTEX_ARRAY);

			glVertexPointer(3, GL_FLOAT, 3*sizeof(float), 0);
    
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, model.getTextureID());//Bind texture to sampler 0

			glDrawElements(GL_TRIANGLES, 3*model.getFaceCount(), GL_UNSIGNED_SHORT, 0);

			glBindTexture(GL_TEXTURE_2D, 0); //Unbind texture

			glDisableClientState(GL_VERTEX_ARRAY);
    
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Unbind index buffer
			glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind vertex buffer
    
			glBindVertexArray(0); //Unbind vertex array

			glUseProgram(0); //Disable shader program

			//Draw vertex normals as lines
			/*
			glColor3f(0.0, 1.0, 0.0);
			for (int c = 0; c < data.vertexCount; c++)
			{
				glBegin(GL_LINES);
					glVertex3f(data.vertexData[c*3], data.vertexData[c*3+1], data.vertexData[c*3+2]);
					glVertex3f(data.vertexData[c*3]+0.01*data.vertexNormalData[c*3], 
                           data.vertexData[c*3+1]+0.01*data.vertexNormalData[c*3+1], 
                           data.vertexData[c*3+2]+0.01*data.vertexNormalData[c*3+2] );
				glEnd();
			}
			 */
			
	    glPopMatrix();
	}
	//End of modelDrawing
	//#################################
}
/*
void Renderer::drawTerrain()
{
			glUseProgram(LevelManager::get().getTerrain().getModel().getShaderProgram()); //Use shader program

			glsl_parameter_locations glsl_locations = LevelManager::get().getTerrain().getModel().getGlslParameterLocations();
			//#####################################
			//#Calculate and pass values for GLSL parameters
			//#####################################
			//Pass matrices to shaders
			GLfloat mp[16];
			GLfloat mv[16];
			glGetFloatv(GL_PROJECTION_MATRIX, mp);
			glGetFloatv(GL_MODELVIEW_MATRIX, mv);
			glUniformMatrix4fv(glsl_locations.location_projection_matrix, 1, GL_FALSE, mp);
			glUniformMatrix4fv(glsl_locations.location_modelview_matrix, 1, GL_FALSE, mv);

			//#Pass light parameters to shaders#
			glUniform3f(glsl_locations.location_viewing_point, camera_x, camera_y, camera_z);
			glUniform3f(glsl_locations.location_ambient_light_intensity, ambient_light_intensity[0], ambient_light_intensity[1], ambient_light_intensity[2]);
			glUniform3f(glsl_locations.location_diffuse_light_intensity, diffuse_light_intensity[0], diffuse_light_intensity[1], diffuse_light_intensity[2]);
			glUniform3f(glsl_locations.location_specular_light_intensity, specular_light_intensity[0], specular_light_intensity[1], specular_light_intensity[2]);
			
			//Get model specific reflection constants and pass the values
			vec3 ambient_RC = LevelManager::get().getTerrain().getModel().getAmbientLightReflectionConstant();
			vec3 diffuse_RC = LevelManager::get().getTerrain().getModel().getDiffuseLightReflectionConstant();
			vec3 specular_RC = LevelManager::get().getTerrain().getModel().getSpecularLightReflectionConstant();
			glUniform3f(glsl_locations.location_ambient_light_reflection_constant, ambient_RC.x, ambient_RC.y, ambient_RC.z);			
			glUniform3f(glsl_locations.location_diffuse_light_reflection_constant, diffuse_RC.x, diffuse_RC.y, diffuse_RC.z);			
			glUniform3f(glsl_locations.location_specular_light_reflection_constant, specular_RC.x, specular_RC.y, specular_RC.z);
			//#######################
    
			glBindVertexArray(LevelManager::get().getTerrain().getModel().getVaoID());//Bind vertex array

			glBindBuffer(GL_ARRAY_BUFFER, LevelManager::get().getTerrain().getModel().getVertexVboID());//Bind vertex buffer

			glBindBuffer(GL_ARRAY_BUFFER, LevelManager::get().getTerrain().getModel().getTextureCoordinatesVBOID());//Bind texture coords buffer

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, LevelManager::get().getTerrain().getModel().getIndiceIboID());//Bind index buffer

			glEnableClientState(GL_VERTEX_ARRAY);

			glVertexPointer(3, GL_FLOAT, 3*sizeof(float), 0);
    
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, LevelManager::get().getTerrain().getModel().getTextureID());//Bind texture to sampler 0

			glDrawElements(GL_TRIANGLES, 3*LevelManager::get().getTerrain().getModel().getFaceCount(), GL_UNSIGNED_SHORT, 0);

			glBindTexture(GL_TEXTURE_2D, 0); //Unbind texture

			glDisableClientState(GL_VERTEX_ARRAY);
    
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //Unbind index buffer
			glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind vertex buffer
    
			glBindVertexArray(0); //Unbind vertex array

			glUseProgram(0); //Disable shader program
}
*/
float Renderer::getCameraX()
{
	return camera_x;
}

float Renderer::getCameraY()
{
	return camera_y;
}

float Renderer::getCameraZ()
{
	return camera_z;
}

void Renderer::setCameraX(float new_x)
{
	camera_x = new_x;
}

void Renderer::setCameraY(float new_y)
{
	camera_y = new_y;
}

void Renderer::setCameraZ(float new_z)
{
	camera_z = new_z;
}

void Renderer::rotateCameraRight()
{
	Player::get().increaseAngle();
	Player::get().setVX( 2.0 * sin(Player::get().getAngle()) );
    Player::get().setVZ(-2.0*cos(Player::get().getAngle()));
	camera_x = Player::get().getX() - Player::get().getVX();
    camera_z = Player::get().getZ() - Player::get().getVZ();
}

void Renderer::rotateCameraLeft()
{
	Player::get().decreaseAngle();
    Player::get().setVX(2.0*sin(Player::get().getAngle()));
    Player::get().setVZ(-2.0*cos(Player::get().getAngle()));
	camera_x = Player::get().getX() - Player::get().getVX();
    camera_z = Player::get().getZ() - Player::get().getVZ();
}