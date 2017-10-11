#include "stdafx.h"
#include "Renderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

Renderer Renderer::instance = Renderer();

void Renderer::init(HDC deviceCxtHandle)
{
	ambient_light_intensity[0] = 0.4; // Ambient light color/intensity
	ambient_light_intensity[1] = 0.4;
	ambient_light_intensity[2] = 0.4;
	//Init other lights temporarily
	diffuse_light_intensity[0] = 0.3; // Diffuse light color/intensity
	diffuse_light_intensity[1] = 0.3;
	diffuse_light_intensity[2] = 0.3;
	specular_light_intensity[0] = 0.2; // Specular light color/intensity
	specular_light_intensity[1] = 0.2;
	specular_light_intensity[2] = 0.2;
	//End of temporary

	deviceContextHandle = deviceCxtHandle;

	camera_x = 0.0;
	camera_y = 4.0;
	camera_z = 2.0;

	glClearColor(0.0, 0.0, 0.1, 0.0);
    
	glEnable(GL_DEPTH_TEST); // Z-Buffer
	
	glFrontFace(GL_CCW); // Our models triangles vertices are in CCW order so GL_CCW side is front
	glCullFace(GL_BACK); // Enable culling of backfacing triangles
    
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void Renderer::drawScene()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
   
	//gluLookAt(camera_x, camera_y, camera_z, Player::get().getX(), Player::get().getY(), Player::get().getZ(), 0.0, 1.0, 0.0);
	//glPushMatrix();
	//drawTerrain();

	//Player::get().draw();

    drawModels();
	
	//UI::get().draw();
	
    SwapBuffers(deviceContextHandle);

    glFlush();    
}


void Renderer::drawModels()
{
	std::vector<GameObject>& gameObjects = LevelManager::get().getTerrain().getGameObjects();
	// Add other game objects to this list if they should be drawn

	

	for (std::vector<GameObject>::const_iterator it = gameObjects.begin(); it < gameObjects.end(); it++)
	{
		//glPushMatrix();
			Model& model = it->getModel(); 
			
			glUseProgram(model.getShaderProgram()); // Use shader program

			GLfloat scale = it->getScale(); // Scaling and transforming must be done the same way to terrain squares which are used for col. detec.
			vec3 position = it->getPosition();
			model_data data = model.getModelData();
			
			// Temporary normal rendering
			// As we need to negate normals to see them we know that normals are calculated to the wrong side of the triangle
			// TODO fix normal calculation
			// Seems that data passes through shaders. If we move glUseProgram below this it doesnt, but for some reason transformations dont match
			float* v = data.vertexData;
			float* n = data.vertexNormalData;
			
			// Testing
			glLoadIdentity();
			glTranslatef(position.x, position.y, position.z);
			glScalef(scale, scale, scale);
			GLfloat currentModelView[16];
			glGetFloatv(GL_MODELVIEW_MATRIX, currentModelView);
			// If currentModelView != modelViewMatrix that is the problem
			GLfloat currentProjectionMatrix[16];
			glGetFloatv(GL_PROJECTION_MATRIX, currentProjectionMatrix);
			// If currentProjectionMatrix != projectionMatrix that is the problem
			// TODO add condtition which launches breakpoint if so
			// End testing

			for (int c = 0; c < data.vertexCount; c++)
			{
				//glColor3f(0.0, 1.0, 0.23);
				glBegin(GL_LINES);
				glVertex3f(v[c * 3], v[c * 3 + 1], v[c * 3 + 2]);
				glVertex3f(v[c * 3] + 0.8 * n[c * 3],
					v[c * 3 + 1] + 0.8 * n[c * 3 + 1],
					v[c * 3 + 2] + 0.8 * n[c * 3 + 2]);
				glEnd();
			}
			//glPopMatrix();
			// End of temp

			
			// Calculate model matrix
			glm::mat4 modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
			modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z)) * modelMatrix;
			
			// Get view matrix
			//GLfloat v[16];
			//glGetFloatv(GL_MODELVIEW_MATRIX, v);
			//glm::mat4 viewMatrix = glm::make_mat4x4(v);
			glm::vec3 cam(camera_x, camera_y, camera_z);
			glm::vec3 target(Player::get().getX(), Player::get().getY(), Player::get().getZ());
			glm::mat4 viewMatrix = glm::lookAt(cam, target, glm::vec3(0.0, 1.0, 0.0));

			//glTranslatef(position.x, position.y, position.z);
			//glScalef(scale, scale, scale);
			//glMultMatrixf(glm::value_ptr(modelMatrix));

			glsl_parameter_locations glsl_locations = model.getGlslParameterLocations(); // Get parameter glsl locations
			//#################################################
			//# Calculate and pass values for GLSL parameters #
			//#################################################

			// Projection matrix
			//GLfloat mp[16];
			//glGetFloatv(GL_PROJECTION_MATRIX, mp);
			//glm::mat4 projectionMatrix = glm::make_mat4x4(mp);
			//glUniformMatrix4fv(glsl_locations.location_projection_matrix, 1, GL_FALSE, mp);
			glm::mat4 projectionMatrix = glm::perspective(45.0f, 1.0f, 0.5f, 200.f);
			glUniformMatrix4fv(glsl_locations.location_projection_matrix, 1, GL_FALSE, &projectionMatrix[0][0]);

			// Model View matrix
			glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
			glUniformMatrix4fv(glsl_locations.location_modelview_matrix, 1, GL_FALSE, &modelViewMatrix[0][0]);

			// Model matrix
			glUniformMatrix4fv(glsl_locations.location_model_matrix, 1, GL_FALSE, &modelMatrix[0][0]);

			// View matrix
			glUniformMatrix4fv(glsl_locations.location_view_matrix, 1, GL_FALSE, &viewMatrix[0][0]);

			// Normal matrix, 3x3
			glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelViewMatrix));
			glUniformMatrix3fv(glsl_locations.location_normal_matrix, 1, GL_FALSE, &normalMatrix[0][0]);

			// Camera
			glUniform3f(glsl_locations.location_viewing_point, camera_x, camera_y, camera_z);

			// Pass light parameters to shaders
			glUniform3f(glsl_locations.location_ambient_light_intensity, ambient_light_intensity[0], ambient_light_intensity[1], ambient_light_intensity[2]);
			glUniform3f(glsl_locations.location_diffuse_light_intensity, diffuse_light_intensity[0], diffuse_light_intensity[1], diffuse_light_intensity[2]);
			glUniform3f(glsl_locations.location_specular_light_intensity, specular_light_intensity[0], specular_light_intensity[1], specular_light_intensity[2]);
			
			// Get model specific reflection constants and pass the values
			vec3 ambient_RC = model.getAmbientLightReflectionConstant();
			vec3 diffuse_RC = model.getDiffuseLightReflectionConstant();
			vec3 specular_RC = model.getSpecularLightReflectionConstant();
			glUniform3f(glsl_locations.location_ambient_light_reflection_constant, ambient_RC.x, ambient_RC.y, ambient_RC.z);			
			glUniform3f(glsl_locations.location_diffuse_light_reflection_constant, diffuse_RC.x, diffuse_RC.y, diffuse_RC.z);			
			glUniform3f(glsl_locations.location_specular_light_reflection_constant, specular_RC.x, specular_RC.y, specular_RC.z);
        
			//glColor3f(1.0, 0.0, 0.0);
    
			glBindVertexArray(model.getVaoID()); // Bind vertex array

			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, model.getVertexVboID()); // Bind vertex buffer
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // Set-up pointer

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.getIndiceIboID()); // Bind index buffer
    
			glEnableVertexAttribArray(1); // Bind location data to texture_coordinates shader-param
			glBindBuffer(GL_ARRAY_BUFFER, model.getTextureCoordinatesVBOID()); // Bind texture coords buffer
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0); // Set-up pointer
			
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, model.getNormalVboID()); // Bind normal buffer
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // Set-up pointer

			glEnableClientState(GL_VERTEX_ARRAY);

			glVertexPointer(3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, model.getTextureID()); // Bind texture to sampler 0

			glDrawElements(GL_TRIANGLES, 3 * model.getFaceCount(), GL_UNSIGNED_SHORT, 0);
			//glDrawArrays(GL_TRIANGLES, 0, 3 * model.getVertexCount());
			
			// Done drawing, unbind

			glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture

			glDisableClientState(GL_VERTEX_ARRAY);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
    
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind index buffer
			glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind vertex buffer
    
			glBindVertexArray(0); // Unbind vertex array

			glUseProgram(0); // Disable shader program

			//Draw vertex normals as lines
			
			//glPushMatrix();
			//glMatrixMode(GL_PROJECTION);    /* prepare for and then */
			//glLoadIdentity();               /* define the projection */
			//gluPerspective(45, 1, 0.5, 20);

			//glMatrixMode(GL_MODELVIEW);
			//glLoadIdentity();
			
			//glLoadMatrixf(&viewMatrix[0][0]);

			//gluLookAt(camera_x, camera_y, camera_z, Player::get().getX(), Player::get().getY(), Player::get().getZ(), 0.0, 1.0, 0.0);
			

	}
	//End of modelDrawing
	//#################################
}

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
