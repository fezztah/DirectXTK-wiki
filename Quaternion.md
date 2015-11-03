A rotation represented as a four component vector modeled after the [XNA Game Studio](https://msdn.microsoft.com/en-us/library/microsoft.xna.framework.quaternion.aspx) math library.

# Header

    #include <SimpleMath.h>

# Initialization

    using namespace DirectX::SimpleMath;

    Quaternion q;                     // Creates the identity quaternion [0, 0, 0, 1]
    Quaternion q(0, 0, 0, 1);         // Creates a quaternion [0, 0, 0, 1]
    Quaternion q( Vector3(0,0,0), 1); // Creates a quaternion [0, 0, 0, 1]
    Quaternion q( Vector4(0,0,0,1) ); // Creates a quaternion [0, 0, 0, 1]

    float arr[4] = { 0, 0, 0, 1 };
    Quaternion q(arr);                // Creates a quaternion [0, 0, 0, 1]

# Fields
* *x* vector component of the quaternion
* *y* vector component of the quaternion
* *z* vector component of the quaternion
* *w* scalar component of the quaternion

# Methods
* Comparison operators: ``==`` and ``!=``
* Assignment operators: ``=``, ``+=``, ``-=``, ``*=``, ``/=``
* Unary operators: ``+``, ``-``
* Binary operators: ``+``, ``-``, ``*``, ``/``
* **Length**
* **LengthSquared**
* **Normalize**: Normalizes the quaternion. _Note that only normalized quaternions correspond to 3D rotations._
* **Conjugate**
* **Inverse**
* **Dot**

# Statics
* **CreateFromAxisAngle**
* **CreateFromYawPitchRoll**
* **CreateFromRotationMatrix**
* **Lerp**: Linear interpolation
* **Slerp**: Spherical linear interpolation
* **Concatenate**: Concatenates two quaternion rotations. _Note: ``Concatenate(q1,q2)`` is equivalent to ``q2*q1``_.

# Constants
* **Identity**: The identity quaternion [0, 0, 0, 1]

# Remark
Quaternion can freely convert to and from a ``XMFLOAT4`` and ``XMVECTOR``

# Further Reading
[Quaternions and spatial rotation](https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation)  

Jonathan Blow, "Understanding Slerp, Then Not Using It", The Inner Product, April 2004 [link](http://number-none.com/product/Understanding%20Slerp,%20Then%20Not%20Using%20It/)

David Eberly, "Quaternion Algebra and Calculus" [link](http://www.geometrictools.com/Documentation/Quaternions.pdf)

Ken Shoemake, "Quaternions", Department of Computer and Information Science, University of Pennsylvania [link](http://www.cs.ucr.edu/~vbz/resources/quatut.pdf)
