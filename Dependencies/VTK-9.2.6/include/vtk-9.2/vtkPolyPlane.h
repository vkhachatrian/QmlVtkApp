/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPolyPlane.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   vtkPolyPlane
 * @brief   Implicit function that is generated by extrusion of a polyline along the Z axis
 *
 * vtkPolyPlane is, as the name suggests, an extrusion of a vtkPolyLine.
 * The extrusion direction is assumed to be the Z vector. It can be used in
 * combination with a vtkCutter to cut a dataset with a polyplane.
 * vtkPolyPlane is a concrete implementation of the abstract class
 * vtkImplicitFunction.
 *
 * @todo
 * Generalize to extrusions along arbitrary directions.
 */

#ifndef vtkPolyPlane_h
#define vtkPolyPlane_h

#include "vtkCommonDataModelModule.h" // For export macro
#include "vtkImplicitFunction.h"

class vtkPolyLine;
class vtkDoubleArray;

class VTKCOMMONDATAMODEL_EXPORT vtkPolyPlane : public vtkImplicitFunction
{
public:
  /**
   * Construct plane passing through origin and normal to z-axis.
   */
  static vtkPolyPlane* New();

  vtkTypeMacro(vtkPolyPlane, vtkImplicitFunction);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  ///@{
  /**
   * Evaluate plane equation for point x[3].
   */
  using vtkImplicitFunction::EvaluateFunction;
  double EvaluateFunction(double x[3]) override;
  ///@}

  /**
   * Evaluate function gradient at point x[3].
   */
  void EvaluateGradient(double x[3], double g[3]) override;

  ///@{
  /**
   * Set/get point through which plane passes. Plane is defined by point
   * and normal.
   */
  virtual void SetPolyLine(vtkPolyLine*);
  vtkGetObjectMacro(PolyLine, vtkPolyLine);
  ///@}

  /**
   * Override GetMTime to include the polyline
   */
  vtkMTimeType GetMTime() override;

protected:
  vtkPolyPlane();
  ~vtkPolyPlane() override;

  void ComputeNormals();

  double ExtrusionDirection[3];
  vtkPolyLine* PolyLine;
  vtkTimeStamp NormalComputeTime;
  vtkDoubleArray* Normals;
  vtkIdType ClosestPlaneIdx;

private:
  vtkPolyPlane(const vtkPolyPlane&) = delete;
  void operator=(const vtkPolyPlane&) = delete;
};

#endif
