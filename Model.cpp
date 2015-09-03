#include "stable.h"
#include "Model.h"


Model::Model()
{
}


Model::~Model()
{
}

////utilities
void Model::GenerateTengentSpace(){

	/*for (unsigned int i = 0; i < Indices.size(); i += 3) {
		Vertex& v0 = Vertices[Indices[i]];
		Vertex& v1 = Vertices[Indices[i + 1]];
		Vertex& v2 = Vertices[Indices[i + 2]];

		Vector3f Edge1 = v1.m_pos - v0.m_pos;
		Vector3f Edge2 = v2.m_pos - v0.m_pos;

		float DeltaU1 = v1.m_tex.x - v0.m_tex.x;
		float DeltaV1 = v1.m_tex.y - v0.m_tex.y;
		float DeltaU2 = v2.m_tex.x - v0.m_tex.x;
		float DeltaV2 = v2.m_tex.y - v0.m_tex.y;

		float f = 1.0f / (DeltaU1 * DeltaV2 - DeltaU2 * DeltaV1);

		Vector3f Tangent, Bitangent;

		Tangent.x = f * (DeltaV2 * Edge1.x - DeltaV1 * Edge2.x);
		Tangent.y = f * (DeltaV2 * Edge1.y - DeltaV1 * Edge2.y);
		Tangent.z = f * (DeltaV2 * Edge1.z - DeltaV1 * Edge2.z);

		Bitangent.x = f * (-DeltaU2 * Edge1.x - DeltaU1 * Edge2.x);
		Bitangent.y = f * (-DeltaU2 * Edge1.y - DeltaU1 * Edge2.y);
		Bitangent.z = f * (-DeltaU2 * Edge1.z - DeltaU1 * Edge2.z);

		v0.m_tangent += Tangent;
		v1.m_tangent += Tangent;
		v2.m_tangent += Tangent;
	}*/
	m_tangent.resize(m_polygons.size()*3);
	m_bitengent.resize(m_polygons.size() * 3);
    //std::vector<Vec3f> face_tangent;
    //std::vector<Vec3f> face_binormal;
    //face_tangent.resize(m_polygons.size());
    //face_binormal.resize(m_polygons.size());
    if (haveUV){
		for (unsigned i = 0; i < m_polygons.size(); i++)
		{
			Vec3f Edge1 = m_verts[m_polygons[i][1]] - m_verts[m_polygons[i][0]];
			Vec3f Edge2 = m_verts[m_polygons[i][2]] - m_verts[m_polygons[i][0]];

			Vec2f DeltaUV1 = m_uv_raw[i][1] - m_uv_raw[i][0];
			Vec2f DeltaUV2 = m_uv_raw[i][2] - m_uv_raw[i][0];

			float f = 1.0f / (DeltaUV1.x() * DeltaUV2.y() - DeltaUV2.x() * DeltaUV1.y());

			Vec3f Tangent, Bitangent;

            Tangent = Edge1*DeltaUV1.x()-Edge2*DeltaUV2.y();
            //Tangent.normalize();
            Bitangent = m_vert_normal[m_polygons[i][0]].cross(Tangent);

            //face_tangent[i] = Tangent;
            m_tangent[i][0] = Tangent;
            m_tangent[i][1] = Tangent;
            m_tangent[i][2] = Tangent;

            m_bitengent[i][0] = Bitangent;
            m_bitengent[i][1] = Bitangent;
            m_bitengent[i][2] = Bitangent;

		}
	}
}


//// transformation ///////////////////////

void Model::Translate(Vec3f &transVector, Trans_Space space){
    Mat4f transMat = Mat4f(
        1.0, 0.0, 0.0, transVector.x(),
        0.0, 1.0, 0.0, transVector.y(),
        0.0, 0.0, 1.0, transVector.z(),
        0.0,0.0,0.0,1.0
        );
    if (space == TRANS_WORLD)
    {
        Translate_Matrix = transMat*Translate_Matrix;
        //translate center
        Transformation_Matrix = transMat*Transformation_Matrix;
        m_center = HomoGenTransformation(transMat, m_center);
        pivot = HomoGenTransformation(transMat, pivot);
    }
    else if (space == TRANS_PARENT)
    {
        transMat = Parent_Matrix.Inverse()*transMat*Parent_Matrix;
        Translate_Matrix = transMat*Translate_Matrix;
        Transformation_Matrix = transMat*Transformation_Matrix;
        m_center = HomoGenTransformation(transMat, m_center);
        pivot = HomoGenTransformation(transMat, pivot);
    }
    else if (space == TRANS_SELF)   //rotation
    {
        transMat = Rotation_Matrix*transMat*Rotation_Matrix.Inverse();
        Translate_Matrix = transMat*Translate_Matrix;
        Transformation_Matrix = transMat*Transformation_Matrix;
        m_center = HomoGenTransformation(transMat, m_center);
        pivot = HomoGenTransformation(transMat, pivot);
    }
    else if(space ==TRANS_PIVOT){
        transMat = Rotation_Matrix*transMat*Rotation_Matrix.Inverse();
        Translate_Matrix = transMat*Translate_Matrix;
        Transformation_Matrix = transMat*Transformation_Matrix;
        m_center = HomoGenTransformation(transMat, m_center);
        pivot = HomoGenTransformation(transMat, pivot);
    }
    else{
        std::cerr << "Flag invalid!" << std::endl;
    }
}
void Model::Rotation(float degree, Vec3f &axis, Trans_Space space){
    float r_radius = degree / 180 * PI;
    Mat4f Mat_Rotate = Mat4f(
        ((1 - cos(r_radius))*axis.x()*axis.x()) + cos(r_radius), ((1 - cos(r_radius))*axis.x()*axis.y()) - axis.z()*sin(r_radius), ((1 - cos(r_radius))*axis.x()*axis.z()) + axis.y()*sin(r_radius), 0.0f,
        ((1 - cos(r_radius))*axis.x()*axis.y()) + axis.z()*sin(r_radius), ((1 - cos(r_radius))*axis.y()*axis.y()) + cos(r_radius), ((1 - cos(r_radius))*axis.y()*axis.z()) - axis.x()*sin(r_radius), 0.0f,
        ((1 - cos(r_radius))*axis.x()*axis.z()) - axis.y()*sin(r_radius), ((1 - cos(r_radius))*axis.z()*axis.y()) + axis.x()*sin(r_radius), ((1 - cos(r_radius))*axis.z()*axis.z()) + cos(r_radius), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );
    Mat4f Translate_M = Mat4f(
        1.0f, 0.0f, 0.0f, -m_center.x(),
        0.0f, 1.0f, 0.0f, -m_center.y(),
        0.0f, 0.0f, 1.0f, -m_center.z(),
        0.0f, 0.0f, 0.0f, 1.0f
        );
    //Rotation_M = Parent_M.Inverse()*Mat_Rotate*Parent_M*Temp_RMatrix;

    if (space == TRANS_WORLD)
    {
        Rotation_Matrix = Mat_Rotate*Rotation_Matrix;
        Transformation_Matrix = Translate_M.Inverse()*Mat_Rotate*Translate_M*Transformation_Matrix;
    }
    else if (space==TRANS_PARENT)
    {
        Rotation_Matrix = Parent_Matrix.Inverse()*Mat_Rotate*Parent_Matrix*Rotation_Matrix;
        Transformation_Matrix = Translate_M.Inverse()*Parent_Matrix.Inverse()*Mat_Rotate*Parent_Matrix*Translate_M*Transformation_Matrix;
    }
    else if (space == TRANS_SELF)
    {
        Rotation_Matrix = Parent_Matrix.Inverse()*Mat_Rotate*Parent_Matrix*Rotation_Matrix;
        Transformation_Matrix = Translate_M.Inverse()*Parent_Matrix.Inverse()*Mat_Rotate*Parent_Matrix*Translate_M*Transformation_Matrix;
    }
    else if (space ==TRANS_PIVOT){
        Mat4f Pivot_Trans=Mat4f(
                    1.0f, 0.0f, 0.0f, -pivot.x(),
                    0.0f, 1.0f, 0.0f, -pivot.y(),
                    0.0f, 0.0f, 1.0f, -pivot.z(),
                    0.0f, 0.0f, 0.0f, 1.0f
                    );

        Transformation_Matrix = Pivot_Trans.Inverse()*Rotation_Matrix*Mat_Rotate*Rotation_Matrix.Inverse()*Pivot_Trans*Transformation_Matrix;
        Rotation_Matrix = Mat_Rotate*Rotation_Matrix;
    }
    else
    {
        std::cerr << "Flag invalid!" << std::endl;
    }
    /*Rotation_Matrix = Parent_Matrix.Inverse()*Mat_Rotate*Parent_Matrix*Rotation_Matrix;
    Transformation_Matrix = Translate.Inverse()*Parent_Matrix.Inverse()*Mat_Rotate*Parent_Matrix*Translate*Transformation_Matrix;*/
}

void Model::RelativeRotation(float degree, Vec3f &axis, Trans_Space space){
    float r_radius = degree / 180 * PI;
    Mat4f Mat_Rotate = Mat4f(
        ((1 - cos(r_radius))*axis.x()*axis.x()) + cos(r_radius), ((1 - cos(r_radius))*axis.x()*axis.y()) - axis.z()*sin(r_radius), ((1 - cos(r_radius))*axis.x()*axis.z()) + axis.y()*sin(r_radius), 0.0f,
        ((1 - cos(r_radius))*axis.x()*axis.y()) + axis.z()*sin(r_radius), ((1 - cos(r_radius))*axis.y()*axis.y()) + cos(r_radius), ((1 - cos(r_radius))*axis.y()*axis.z()) - axis.x()*sin(r_radius), 0.0f,
        ((1 - cos(r_radius))*axis.x()*axis.z()) - axis.y()*sin(r_radius), ((1 - cos(r_radius))*axis.z()*axis.y()) + axis.x()*sin(r_radius), ((1 - cos(r_radius))*axis.z()*axis.z()) + cos(r_radius), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );
    Mat4f Translate_M = Mat4f(
        1.0f, 0.0f, 0.0f, -m_center.x(),
        0.0f, 1.0f, 0.0f, -m_center.y(),
        0.0f, 0.0f, 1.0f, -m_center.z(),
        0.0f, 0.0f, 0.0f, 1.0f
        );
    //Rotation_M = Parent_M.Inverse()*Mat_Rotate*Parent_M*Temp_RMatrix;
    /*Rotation_Matrix = Mat_Rotate*Rotation_Matrix;
    Transformation_Matrix = Translate_M.Inverse()*Mat_Rotate*Translate_M*Transformation_Matrix;*/

    Rotation_Matrix = Parent_Matrix.Inverse()*Mat_Rotate*Parent_Matrix*Rotation_Matrix;
    Transformation_Matrix = Translate_M.Inverse()*Parent_Matrix.Inverse()*Mat_Rotate*Parent_Matrix*Translate_M*Transformation_Matrix;
}
void Model::Scale(float percentage){
	Mat4f Scale_Mat = Mat4f(
		1.0+percentage,0.0,0.0,0.0,
		0.0, 1.0 + percentage, 0.0, 0.0,
		0.0, 0.0, 1.0 + percentage, 0.0, 
		0.0,0.0,0.0,1.0
		);
	Mat4f Translate_M = Mat4f(
		1.0f, 0.0f, 0.0f, -m_center.x(),
		0.0f, 1.0f, 0.0f, -m_center.y(),
		0.0f, 0.0f, 1.0f, -m_center.z(),
		0.0f, 0.0f, 0.0f, 1.0f
		);
	Scale_Matrix = Scale_Mat*Scale_Matrix;

	Transformation_Matrix = Translate_M.Inverse()*Rotation_Matrix*Scale_Mat*Rotation_Matrix.Inverse()*Translate_M*Transformation_Matrix;
}


void Model::Forward(ForwardType flag){
    if(flag==F_DRAW){//draw
        Translate(Vec3f(0.0,0.0,m_max_bounding.z()-m_min_bounding.z()),TRANS_SELF);
        //Translate(Vec3f(0.0,0.0,3),TRANS_SELF);
        ClassicDraw(SMOOTH,UV_RAW);
        //VBODraw();
    }
    else{           //not draw
        Translate(Vec3f(0.0,0.0,m_max_bounding.z()-m_min_bounding.z()),TRANS_SELF);
        //Translate(Vec3f(0.0,0.0,3),TRANS_SELF);
    }
}

////// draw //////////////////////////
void Model::ClassicDraw(int drawType,int uv_type=UV_LACK){
//    glUseProgram(materialID);
	glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
	/*glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);*/
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glLoadMatrixf(Transformation_Matrix.ToArray());
	glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_TRIANGLES);
	for (unsigned int i = 0; i < m_polygons.size(); i++){
		if (drawType==FLAT)
		{
			glNormal3fv(m_face_normal[i].getPtr());
		}
		for (unsigned int j = 0; j < m_polygons[i].size(); j++){
			if (drawType == SMOOTH)
			{
				glNormal3fv(m_vert_normal[m_polygons[i][j]].getPtr());
			}
			
			if (uv_type==UV_VERTEX){
				glTexCoord2fv(m_uv_coord[m_polygons[i][j]].getPtr());
			}
			else if (uv_type == UV_RAW)
			{
				glTexCoord2fv(m_uv_raw[i][j].getPtr());
			}

			glVertex3fv(m_verts[m_polygons[i][j]].getPtr());
			
		}
	}
	glEnd();
}
void Model::VBODraw(){

	//pass transformation matrix
    /*GLuint ModelMatrix=glGetUniformLocation(materialID, "ModelViewMatrix");
	glUniformMatrix4fv(ModelMatrix,16*sizeof(float),false,Transformation_Matrix.ToArray());*/
    glUseProgram(materialID);

    Mat4f ModelViewProjectionMat=ProjectionViewMatrix*Transformation_Matrix;
    GLuint MatrixID = glGetUniformLocation(materialID, "MVP");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, ModelViewProjectionMat.ToArray());

    GLuint ModelMatrixID = glGetUniformLocation(materialID, "M");
    glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, ModelViewProjectionMat.ToArray());

    Mat3f ModelViewMatrix3x3=ViewnMat*Mat3f(Rotation_Matrix.xx(),Rotation_Matrix.xy(),Rotation_Matrix.xz(),Rotation_Matrix.yx(),Rotation_Matrix.yy(),Rotation_Matrix.yz(),Rotation_Matrix.zx(),Rotation_Matrix.zy(),Rotation_Matrix.zz());
    GLuint ModelView3x3MatrixID = glGetUniformLocation(materialID, "MV3x3");
    glUniformMatrix3fv(ModelView3x3MatrixID,1,GL_FALSE,ModelViewMatrix3x3.ToArray());

    // Bind our diffuse texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pMaterial->getTextureID(0));
            // Set our "DiffuseTextureSampler" sampler to user Texture Unit 0
     glUniform1i(pMaterial->getTextureID(0), 0);

     glActiveTexture(GL_TEXTURE1);
     glBindTexture(GL_TEXTURE_2D, pMaterial->getTextureID(1));
             // Set our "DiffuseTextureSampler" sampler to user Texture Unit 0
      glUniform1i(pMaterial->getTextureID(1), 1);

      glActiveTexture(GL_TEXTURE2);
      glBindTexture(GL_TEXTURE_2D, pMaterial->getTextureID(2));
              // Set our "DiffuseTextureSampler" sampler to user Texture Unit 0
       glUniform1i(pMaterial->getTextureID(2), 2);

    //VBO
    //const unsigned int size = m_polygons.size() * 9;
    //std::vector<float> vertex_array;
    //vertex_array.resize(size);
    //for (unsigned int i = 0; i < m_polygons.size(); i++){
        //vertex_array[i * 3] = m_verts[m_polygons[i][0]].x();
        //vertex_array[i * 3 + 1] = m_verts[m_polygons[i][0]].y();
        //vertex_array[i * 3 + 2] = m_verts[m_polygons[i][0]].z();
        //vertex_array[i * 3 + 3] = m_verts[m_polygons[i][1]].x();
        //vertex_array[i * 3 + 4] = m_verts[m_polygons[i][1]].y();
        //vertex_array[i * 3 + 5] = m_verts[m_polygons[i][1]].z();
        //vertex_array[i * 3 + 6] = m_verts[m_polygons[i][2]].x();
        //vertex_array[i * 3 + 7] = m_verts[m_polygons[i][2]].y();
        //vertex_array[i * 3 + 8] = m_verts[m_polygons[i][2]].z();
    //}
	//vertexArray
	GLuint VertexArray;
	glGenBuffers(1,&VertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, VertexArray);
    glBufferData(GL_ARRAY_BUFFER, vbo_size*3*sizeof(float), vbo_vertex, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VertexArray);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0, 
		(void*)0
		);

    //uv
    GLuint UVArray;
    glGenBuffers(1, &UVArray);
    glBindBuffer(GL_ARRAY_BUFFER,UVArray);
    glBufferData(GL_ARRAY_BUFFER,vbo_size*2*sizeof(float),vbo_uv,GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,UVArray);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
        );


	//normal
    //std::vector<float> normal_array;
    //normal_array.resize(size);
    //for (unsigned int i = 0; i < m_polygons.size(); i++){
        //normal_array[i * 3] = m_vert_normal[m_polygons[i][0]].x();
        //normal_array[i * 3 + 1] = m_vert_normal[m_polygons[i][0]].y();
        //normal_array[i * 3 + 2] = m_vert_normal[m_polygons[i][0]].z();
        //normal_array[i * 3 + 3] = m_vert_normal[m_polygons[i][1]].x();
        //normal_array[i * 3 + 4] = m_vert_normal[m_polygons[i][1]].y();
        //normal_array[i * 3 + 5] = m_vert_normal[m_polygons[i][1]].z();
        //normal_array[i * 3 + 6] = m_vert_normal[m_polygons[i][2]].x();
        //normal_array[i * 3 + 7] = m_vert_normal[m_polygons[i][2]].y();
        //normal_array[i * 3 + 8] = m_vert_normal[m_polygons[i][2]].z();
    //}

    //normal
	GLuint NormalArray;
	glGenBuffers(1, &NormalArray);
	glBindBuffer(GL_ARRAY_BUFFER, NormalArray);
    glBufferData(GL_ARRAY_BUFFER,vbo_size*3*sizeof(float), vbo_normal, GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, NormalArray);
	glVertexAttribPointer(
        2,
		3,
		GL_FLOAT,
		GL_FALSE,
		0, 
		(void*)0
		);

    //tangent
    const GLfloat* cvbo_tangent=const_cast<const GLfloat*>(vbo_tangent);
    GLuint TangentArray;
    glGenBuffers(1, &TangentArray);
    glBindBuffer(GL_ARRAY_BUFFER, TangentArray);
    glBufferData(GL_ARRAY_BUFFER,vbo_size*3*sizeof(float), cvbo_tangent, GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, TangentArray);
    glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
        );

    GLuint BinormalArray;
    glGenBuffers(1, &BinormalArray);
    glBindBuffer(GL_ARRAY_BUFFER, BinormalArray);
    glBufferData(GL_ARRAY_BUFFER,vbo_size*3*sizeof(float), vbo_binormal, GL_STATIC_DRAW);

    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, BinormalArray);
    glVertexAttribPointer(
        4,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
        );



	//texture coordinate
	/*GLuint TextureCoordinate;
	glGenBuffers(1, &TextureCoordinate);*/
	//glBindBuffer();


    glDrawArrays(GL_TRIANGLES, 0, vbo_size); // Starting from vertex 0; 3 vertices total -> 1 triangle

    glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
            glDisableVertexAttribArray(3);
            glDisableVertexAttribArray(4);
}


//////////geometry ////////////////////////////////////////////////
void Model::getGeometryParameters(){
	/*calculate center point*/

	//check xmin
	auto x_min_elem = min_element(m_verts.begin(), m_verts.end(), [](Vec3f const& s1, Vec3f const& s2)
	{
		return s1.x() < s2.x();
	});
	float x_min = x_min_elem->x();

	auto x_max_elem = max_element(m_verts.begin(), m_verts.end(), [](Vec3f const& s1, Vec3f const& s2)
	{
		return s1.x() < s2.x();
	});
	float x_max = x_max_elem->x();
	//check ymin
	auto y_min_elem = min_element(m_verts.begin(), m_verts.end(), [](Vec3f const& s1, Vec3f const& s2)
	{
		return s1.y() < s2.y();
	});
	float y_min = y_min_elem->y();

	auto y_max_elem = max_element(m_verts.begin(), m_verts.end(), [](Vec3f const& s1, Vec3f const& s2)
	{
		return s1.y() < s2.y();
	});
	float y_max = y_max_elem->y();

	auto z_min_elem = min_element(m_verts.begin(), m_verts.end(), [](Vec3f const& s1, Vec3f const& s2)
	{
		return s1.z() < s2.z();
	});
	float z_min = z_min_elem->z();

	auto z_max_elem = max_element(m_verts.begin(), m_verts.end(), [](Vec3f const& s1, Vec3f const& s2)
	{
		return s1.z() < s2.z();
	});
	float z_max = z_max_elem->z();

	//define bouding box
	m_max_bounding = Vec3f(x_max, y_max, z_max);
	m_min_bounding = Vec3f(x_min, y_min, z_min);
	//get center
	m_center = (m_max_bounding + m_min_bounding) / 2;
}


///utilities ///////////
Vec3f Model::HomoGenTransformation(Mat4f TransMat, Vec3f Vec){
	Vec4f h_Vec(Vec.x(), Vec.y(), Vec.z(), 1.0f);
	h_Vec = TransMat*h_Vec;
	return Vec3f(h_Vec.x(), h_Vec.y(), h_Vec.z());
}

void Model::CreateVertexVBO(){
    //vbo_size=m_polygons.size()*3;
    vbo_vertex=new GLfloat[vbo_size*3];
    for(unsigned int i=0;i<m_polygons.size();i++){
        for(unsigned int j=0;j<3;j++){
            vbo_vertex[9*i+3*j]=m_verts[m_polygons[i][j]].x();
            vbo_vertex[9*i+3*j+1]=m_verts[m_polygons[i][j]].y();
            vbo_vertex[9*i+3*j+2]=m_verts[m_polygons[i][j]].z();
        }
    }
}
void Model::CreateNormalVBO(){
    vbo_normal=new GLfloat[vbo_size*3];
    for(unsigned int i=0;i<m_polygons.size();i++){
        for(unsigned int j=0;j<3;j++){
            vbo_normal[9*i+3*j]=m_vert_normal[m_polygons[i][j]].x();
            vbo_normal[9*i+3*j+1]=m_vert_normal[m_polygons[i][j]].y();
            vbo_normal[9*i+3*j+2]=m_vert_normal[m_polygons[i][j]].z();
        }
    }
}
void Model::CreateTextureVBO(){
    vbo_uv=new GLfloat[vbo_size*2];
    for(unsigned int i=0;i<m_polygons.size();i++){
        for(unsigned int j=0;j<2;j++){
            vbo_uv[6*i+2*j]=m_uv_raw[i][j].x();
            vbo_uv[6*i+2*j+1]=m_uv_raw[i][j].y();;
        }
    }
}
void Model::CreateTangentSpaceVBO(){
    vbo_tangent=new GLfloat[vbo_size*3];
    for(unsigned int i=0;i<m_polygons.size();i++){
        for(unsigned int j=0;j<3;j++){
            vbo_tangent[9*i+3*j]=m_tangent[i][j].x();
            vbo_tangent[9*i+3*j+1]=m_tangent[i][j].y();
            vbo_tangent[9*i+3*j+2]=m_tangent[i][j].z();
        }
    }
    vbo_binormal=new GLfloat[vbo_size*3];
    for(unsigned int i=0;i<m_polygons.size();i++){
        for(unsigned int j=0;j<3;j++){
            vbo_binormal[9*i+3*j]=m_bitengent[i][j].x();
            vbo_binormal[9*i+3*j+1]=m_bitengent[i][j].y();
            vbo_binormal[9*i+3*j+2]=m_bitengent[i][j].z();
        }
    }
}

void Model::setPovitToBottom(){
    pivot=Vec3f((m_max_bounding.x()+m_min_bounding.x())/2,(m_max_bounding.y()+m_min_bounding.y())/2,m_min_bounding.z());
    pivotSpace=Mat4f(
                1.0,0.0,0.0,0.0,
                0.0,1.0,0.0,0.0,
                0.0,0.0,1.0,0.0,
                0.0,0.0,0.0,1.0
                );
}
