//
//  AABox.h - Axis Aligned Boxes
//  hifi
//
//  Added by Brad Hefta-Gaub on 04/11/13.
//  Originally from lighthouse3d. Modified to utilize glm::vec3 and clean up to our coding standards
//
//  Simple axis aligned box class.
//

#ifndef _AABOX_
#define _AABOX_

#include <glm/glm.hpp>

enum BoxFace {
    MIN_X_FACE,
    MAX_X_FACE,
    MIN_Y_FACE,
    MAX_Y_FACE,
    MIN_Z_FACE,
    MAX_Z_FACE
};


enum BoxVertex {
    BOTTOM_LEFT_NEAR   = 0,
    BOTTOM_RIGHT_NEAR  = 1,
    TOP_RIGHT_NEAR     = 2,
    TOP_LEFT_NEAR      = 3,
    BOTTOM_LEFT_FAR    = 4,
    BOTTOM_RIGHT_FAR   = 5,
    TOP_RIGHT_FAR      = 6,
    TOP_LEFT_FAR       = 7
};

const int FACE_COUNT = 6;

class AABox 
{

public:

    AABox(const glm::vec3& corner, float size) : _corner(corner), _size(size, size, size), _topFarLeft(_corner + _size){};
    AABox(const glm::vec3& corner, float x, float y, float z) : _corner(corner), _size(x, y, z), _topFarLeft(_corner + _size){};
    AABox(const glm::vec3& corner, const glm::vec3& size) : _corner(corner), _size(size), _topFarLeft(_corner + _size){};
    AABox() : _corner(0,0,0), _size(0,0,0), _topFarLeft(0,0,0){};
    ~AABox() {};

    void setBox(const glm::vec3& corner, float x, float y, float z) { setBox(corner,glm::vec3(x,y,z)); };
    void setBox(const glm::vec3& corner, const glm::vec3& size);

    // for use in frustum computations
    glm::vec3 getVertexP(const glm::vec3& normal) const;
    glm::vec3 getVertexN(const glm::vec3& normal) const;

    void scale(float scale);

    const glm::vec3& getCorner() const     { return _corner; };
    const glm::vec3& getSize() const       { return _size; };
    const glm::vec3& getCenter() const     { return _center; };
    const glm::vec3& getTopFarLeft() const { return _topFarLeft; };

    glm::vec3 getVertex(BoxVertex vertex) const;

    bool contains(const glm::vec3& point) const;
    bool contains(const AABox& otherBox) const;
    bool expandedContains(const glm::vec3& point, float expansion) const;
    bool expandedIntersectsSegment(const glm::vec3& start, const glm::vec3& end, float expansion) const;
    bool findRayIntersection(const glm::vec3& origin, const glm::vec3& direction, float& distance, BoxFace& face) const;
    bool findSpherePenetration(const glm::vec3& center, float radius, glm::vec3& penetration) const;
    bool findCapsulePenetration(const glm::vec3& start, const glm::vec3& end, float radius, glm::vec3& penetration) const;

private:

    glm::vec3 getClosestPointOnFace(const glm::vec3& point, BoxFace face) const;
    glm::vec3 getClosestPointOnFace(const glm::vec4& origin, const glm::vec4& direction, BoxFace face) const;
    glm::vec4 getPlane(BoxFace face) const;

    static BoxFace getOppositeFace(BoxFace face);

    glm::vec3 _corner;
    glm::vec3 _center;
    glm::vec3 _size;
    glm::vec3 _topFarLeft;
};


#endif
