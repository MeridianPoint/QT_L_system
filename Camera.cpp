#include "stable.h"
#include "Camera.h"


//Camera::Camera()
//{
//}

bool Camera::MovementConstrainSquare(float xmin, float xmax, float ymin, float ymax, Vec3f direction, Vec3f position, float threshold){
	if (position.x()-xmin<=threshold)//collide with minimum x
	{
		if (direction.dot(Vec3f(1.0,0.0,0.0))<0.0)
		{
			return false;
		}
	}
	if (position.y() - ymin <= threshold){//collide with minimum xy
		if (direction.dot(Vec3f(0.0, 1.0, 0.0)) < 0.0)
		{
			return false;
		}
	}
	if (xmax- position.x() <= threshold)//collide with maximum x
	{
		if (direction.dot(Vec3f(-1.0, 0.0, 0.0)) < 0.0)
		{
			return false;
		}
	}
	if (ymax-position.y() <= threshold)//collide with maximum y
	{
		if (direction.dot(Vec3f(0.0, -1.0, 0.0)) < 0.0)
		{
			return false;
		}
	}
	return true;
}

void Camera::ConstrainedTranslate(Vec3f offsetVector){
	Mat4f Trans_Mat(
		1.0f, 0.0f, 0.0f, offsetVector.x(),
		0.0f, 1.0f, 0.0f, offsetVector.y(),
		0.0f, 0.0f, 1.0f, offsetVector.z(),
		0.0f, 0.0f, 0.0f, 1.0f
		);
	Mat4f Idenity(
		1.0,0.0,0.0,0.0,
		0.0,1.0,0.0,0.0,
		0.0,0.0,1.0,0.0,
		0.0,0.0,0.0,1.0
		);
	Trans_Mat = ObjectSpace_Mat.Inverse()*Trans_Mat*ObjectSpace_Mat*Idenity;
	Vec3f dir=Vec3f(Trans_Mat.xw(),Trans_Mat.yw(),Trans_Mat.zw());
	if (MovementConstrainSquare((Xmin + 0.131) * 16, (Xmax + 0.131) * 16, (Ymin -0.048)* 17, (Ymax -0.048)* 17, dir, pos, 0.8f))
	{
		Translate(offsetVector);
	}
}

void Camera::Translate(Vec3f offsetVector){
	//glTranslatef(offsetVector.x(), offsetVector.y(), offsetVector.z());
	Mat4f Trans_Mat(
		1.0f, 0.0f, 0.0f, offsetVector.x(),
		0.0f, 1.0f, 0.0f, offsetVector.y(),
		0.0f, 0.0f, 1.0f, offsetVector.z(),
		0.0f, 0.0f, 0.0f, 1.0f
		);
	Mat4f Translate_Mat(
		1.0f, 0.0f, 0.0f, -pos.x(),
		0.0f, 1.0f, 0.0f, -pos.y(),
		0.0f, 0.0f, 1.0f, -pos.z(),
		0.0f, 0.0f, 0.0f, 1.0f
		); //translate to origin
	//Translate_Matrix = Trans_Mat*Translate_Matrix;
	Transformation_Matrix = Translate_Mat.Inverse()*ObjectSpace_Mat.Inverse()*Trans_Mat*ObjectSpace_Mat*Translate_Mat*Transformation_Matrix;
	Translate_Matrix = Trans_Mat*Translate_Matrix;
	//pos = HomeGenTransform(pos, Translate_Mat.Inverse()*ObjectSpace_Mat.Inverse()*Trans_Mat*ObjectSpace_Mat*Translate_Mat*Trans_Matrix);
	pos = HomeGenTransform(pos, ObjectSpace_Mat.Inverse()*Trans_Mat*ObjectSpace_Mat);
	//CalculateObjSpace();
	//pos += view_dir*offsetVector.length();
}

//void Camera::Rotate(Vec3f angles, Vec3f axis){
/*Mat4f Rotat_Mat();
  Mat4f Translate _Mat(
  1.0f, 0.0f, 0.0f, -pos.x(),
  0.0f, 1.0f, 0.0f, -pos.y(),
  0.0f, 0.0f, 1.0f, -pos.z(),
  0.0f, 0.0f, 0.0f, 1.0f
  ); //translate to origin
  Rotation_Matrix=Rotat_Mat*Rotation_Matrix;
  view_dir=Rotation_Matrix*view_dir;
  Transform_Matrix=Translate _Mat.Inverse()*Rotation_Matrix*Translate _Mat*Transform;
  
*/
//}

void Camera::Rotate(float radius){
	/*Mat4f Rotat_Mat(
		cos(radius), 0.0f,-sin(radius),0.0f,
		0.0f,1.0f,0.0f,0.0f,
		sin(radius), 0.0f, cos(radius),0.0f,
		0.0f,0.0f,0.0f,1.0f

		);*/
	Mat4f Rotat_Mat(
		cos(radius), -sin(radius), 0.0f, 0.0f,
		sin(radius), cos(radius), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f,0.0f,0.0f,1.0f
		);
	Mat4f Translate_Mat(
		1.0f, 0.0f, 0.0f, -pos.x(),
		0.0f, 1.0f, 0.0f, -pos.y(),
		0.0f, 0.0f, 1.0f, -pos.z(),
		0.0f, 0.0f, 0.0f, 1.0f
		); //translate to origin
	Rotation_Matrix = Rotat_Mat*Rotation_Matrix;
	view_dir = HomeGenTransform(view_dir, Rotat_Mat);
	Up_vec = HomeGenTransform(Up_vec, Rotat_Mat);
	Transformation_Matrix = Translate_Mat.Inverse()*Rotation_Matrix*Translate_Mat*Transformation_Matrix;
	//pos = HomeGenTransform(pos, Translate_Mat.Inverse()*Rotation_Matrix*Translate_Mat);
	CalculateObjSpace();
}

Camera::~Camera()
{

}

Mat4f Camera::getProjectionViewMatrix(){
	Mat4f Camera_Trans_Mat = Mat4f(
		1.0,0.0,0.0,-pos.x(),
		0.0,1.0,0.0,-pos.y(),
		0.0,0.0,1.0,-pos.z(),
		0.0,0.0,0.0,1.0
		);
    Vec3f obj_x, obj_y, obj_z;
    Up_vec.normalize();
    obj_z = view_dir;
    obj_z.normalize();
    obj_x = view_dir.cross(Up_vec);
    obj_x.normalize();
    obj_y = obj_x.cross(obj_z);
    Mat4f Camera_Space_Mat = Mat4f(
        obj_x.x(), obj_x.y(), obj_x.z(), 0.0f,
        obj_y.x(), obj_y.y(), obj_y.z(), 0.0f,
        obj_z.x(), obj_z.y(), obj_z.z(), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );

	Mat4f Projection_Mat = Mat4f(
		near_clip / (height*aspect), 0.0, 0.0, 0.0,
		0.0, near_clip / height, 0.0, 0.0,
		0.0, 0.0, 2 * near_clip*far_clip / (near_clip+far_clip),0.0,
		0.0,0.0,-1,0.0
		);
    return Projection_Mat*Camera_Space_Mat*Camera_Trans_Mat;
}

Mat4f Camera::getViewMatrix(){
    Mat4f Camera_Trans_Mat = Mat4f(
        1.0,0.0,0.0,-pos.x(),
        0.0,1.0,0.0,-pos.y(),
        0.0,0.0,1.0,-pos.z(),
        0.0,0.0,0.0,1.0
        );
    Vec3f obj_x, obj_y, obj_z;
    Up_vec.normalize();
    obj_z = view_dir;
    obj_z.normalize();
    obj_x = view_dir.cross(Up_vec);
    obj_x.normalize();
    obj_y = obj_x.cross(obj_z);
    Mat4f Camera_Space_Mat = Mat4f(
        obj_x.x(), obj_x.y(), obj_x.z(), 0.0f,
        obj_y.x(), obj_y.y(), obj_y.z(), 0.0f,
        obj_z.x(), obj_z.y(), obj_z.z(), 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
        );
    return Camera_Space_Mat*Camera_Trans_Mat;
}
Mat4f Camera::getProjectionMatrix(){
    Mat4f Projection_Mat = Mat4f(
        near_clip / (height*aspect), 0.0, 0.0, 0.0,
        0.0, near_clip / height, 0.0, 0.0,
        0.0, 0.0, 2 * near_clip*far_clip / (near_clip+far_clip),0.0,
        0.0,0.0,-1,0.0
        );
    return Projection_Mat;
}

Mat3f Camera::getViewMatrix3x3(){
    Vec3f obj_x, obj_y, obj_z;
    Up_vec.normalize();
    obj_z = view_dir;
    obj_z.normalize();
    obj_x = view_dir.cross(Up_vec);
    obj_x.normalize();
    obj_y = obj_x.cross(obj_z);
    Mat3f Camera_Space_Mat=Mat3f(
        obj_x.x(), obj_x.y(), obj_x.z(),
        obj_y.x(), obj_y.y(), obj_y.z(),
        obj_z.x(), obj_z.y(), obj_z.z()
    );
    return Camera_Space_Mat;
}

inline void Camera::BindMatrix(GLuint ProgramID){
    GLuint ProjectionMatrix=glGetUniformLocation(ProgramID, "ProjectionMatrix");
    Mat4f Proj_Mat=getProjectionViewMatrix();
    glUniformMatrix4fv(ProjectionMatrix,16*sizeof(float),false,Proj_Mat.ToArray());
}

void RotationAroundTarget(float degree, Vec3f rotaion_axis){
//    Mat4f Trans_Mat(
//                1.0f,0.0f,0
//                );
}
