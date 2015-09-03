/*Transformation.h
* base class to provide virtual functions and parameters for transformation
*
*/
#ifndef TRANSFORMATION_H_
#define TRANSFORMATION_H_
#include "stable.h"
#include "Mat4.h"

class Transformation
{
protected:
	//parent matrix
	Mat4f Parent_Matrix;
	//record translate actions
	Mat4f Translate_Matrix;
	//record rotation actions
	Mat4f Rotation_Matrix;
	//record translate actions
	Mat4f Scale_Matrix;
	//record Transformation
	Mat4f Transformation_Matrix;

	//pivot 
	Vec3f pivot;
	Mat4f pivotSpace;
public:
	Transformation();
	//transformation
	/*virtual void Translate(Vec3f offsetVector)=0;
	virtual void Rotate(Vec3f angles, Vec3f axis)=0;
	virtual void Scale(Vec3f scaleVector)=0;
	virtual void Reset() = 0;*/

	//set get
    Mat4f getTranslateMatrix(){
        return Translate_Matrix;
    }

    Mat4f getRotationMatrix(){
        return Rotation_Matrix;
    }
    Mat4f getScaleMatrix(){
        return Scale_Matrix;
    }
    Mat4f getTransformationMatrix(){
        return Transformation_Matrix;
    }
    Vec3f getPivot(){
        return pivot;
    }

	void setPivot(Vec3f PivotPosition);
	void setPivot(Vec3f PivotPosition, Mat4f PivotSpace);


	///virtual transformation functions

	virtual void Translate(Vec3f offsetVector);
	virtual void Rotate(Vec3f angles, Vec3f axis);
	virtual void Scale(Vec3f scaleVector);
	virtual void Reset();
	virtual void Initate();  //set all matrix to identity

	virtual ~Transformation();

	
	Vec3f HomeGenTransform(Vec3f vector, Mat4f TransformMatrix){  //Homogenius transformation(3d to 4d)
		Vec4f Homo_vector(vector.x(), vector.y(), vector.z(), 1);
		Homo_vector = TransformMatrix*Homo_vector;
		Vec3f result_vector = Vec3f(Homo_vector.x(), Homo_vector.y(), Homo_vector.z());
		return result_vector;
	}

    void getTransParameters(Mat4f &ParentMatrix,Mat4f &TranslateMatrix,Mat4f RotationMatrix,Mat4f ScaleMatrix,Mat4f TransformationMatrix,Vec3f pivot_pos,Mat4f pivot_Space){
        //parent matrix
        ParentMatrix= Parent_Matrix;
        //record translate actions
        TranslateMatrix= Translate_Matrix;
        //record rotation actions
        RotationMatrix= Rotation_Matrix;
        //record translate actions
        ScaleMatrix= Scale_Matrix;
        //record Transformation
        TransformationMatrix= Transformation_Matrix;

        //pivot
        pivot_pos= pivot;
        pivot_Space= pivotSpace;
    }
};

#endif 
