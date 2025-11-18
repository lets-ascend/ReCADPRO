# Advanced Line Tools - AutoCAD/Fusion 360 Style

## Overview

Advanced line drawing tools similar to AutoCAD and Fusion 360, providing professional CAD-level line creation and modification capabilities.

## Implemented Tools

### Basic Line Types

1. **Standard Line** (`line`)
   - Two-point line
   - Basic line drawing

2. **Ray** (`ray`)
   - One endpoint, infinite in one direction
   - Click start point, then direction point

3. **Construction Line** (`construction`)
   - Infinite line both ways
   - Used for reference/construction geometry
   - Displayed in gray when construction mode enabled

### Advanced Line Creation

4. **Polyline** (`polyline`)
   - Multiple connected line segments
   - Click to add points
   - Double-click to finish
   - Can be closed (option)

5. **Spline** (`spline`)
   - Smooth curve through control points
   - Cubic spline (degree 3)
   - Click to add control points
   - Double-click to finish

6. **Bezier Curve** (`bezier`)
   - Cubic Bezier curve
   - 4 control points: start, control1, control2, end
   - Click to set each control point

### Geometric Constructions

7. **Tangent Line** (`tangent`)
   - Line tangent to circle/arc
   - Click point, then circle center and radius

8. **Perpendicular Line** (`perpendicular`)
   - Line perpendicular to existing line
   - Click point, then line endpoints

9. **Parallel Line** (`parallel`)
   - Line parallel to existing line
   - Click point, then line endpoints and distance

10. **Offset Line** (`offset`)
    - Offset copy of line
    - Select line, specify offset distance

### Line Modification

11. **Fillet** (`fillet`)
    - Rounded corner between two lines
    - Select two lines, specify radius
    - Creates arc connecting lines

12. **Chamfer** (`chamfer`)
    - Beveled corner between two lines
    - Select two lines, specify distances
    - Creates angled line connecting lines

13. **Trim** (`trim`)
    - Trim line to intersection point
    - Select trim point and line

14. **Extend** (`extend`)
    - Extend line to point
    - Select extend point and line

15. **Break** (`break`)
    - Break line at point
    - Creates two separate line segments

16. **Join** (`join`)
    - Join two lines into one
    - Select two lines (closest endpoints joined)

## Usage

### Construction Mode
- Toggle construction mode checkbox
- Construction lines displayed in gray
- Useful for reference geometry

### Drawing Workflow

**Polyline:**
1. Select Polyline tool
2. Click to add points
3. Double-click to finish
4. Press Escape to cancel

**Spline:**
1. Select Spline tool
2. Click to add control points (minimum 3)
3. Double-click to finish
4. Press Escape to cancel

**Bezier:**
1. Select Bezier tool
2. Click 4 times for start, control1, control2, end
3. Automatically finishes after 4th point

**Ray/Construction:**
1. Select tool
2. Click start point
3. Click direction point
4. Line created

**Geometric Tools:**
1. Select tool (tangent, perpendicular, parallel, etc.)
2. Follow prompts for point/line selection
3. Tool creates line automatically

**Modification Tools:**
1. Select tool (fillet, chamfer, trim, extend, break, join)
2. Select target lines/points
3. Enter parameters (radius, distance, etc.)
4. Modification applied

## Implementation

### C++ Class: `AdvancedLineTools`
- Location: `src/advancedlinetools.h/cpp`
- Registered in QML as `AdvancedLineTools`
- Properties:
  - `currentTool` - Active tool
  - `lineWidth` - Line width
  - `lineColor` - Line color
  - `constructionMode` - Construction mode toggle

### Data Structures

- `LineSegment` - Standard line data
- `PolylineData` - Polyline with multiple points
- `SplineData` - Spline with control points
- `BezierData` - Bezier curve with 4 control points

### QML Integration

- Added to `main.qml` as `advancedLineToolsController`
- Tool buttons in `Sidebar.qml`
- Drawing handlers in `DrawingCanvas.qml`
- Construction mode checkbox in sidebar

## Features

✅ All 16 line tools implemented
✅ Construction mode support
✅ Polyline with multiple points
✅ Spline curves
✅ Bezier curves
✅ Geometric constructions (tangent, perpendicular, parallel, offset)
✅ Line modifications (fillet, chamfer, trim, extend, break, join)
✅ QML UI integration
✅ Drawing state management

## Future Enhancements

- Arc support for fillet
- Visual feedback during drawing
- Snap to endpoints/midpoints
- Line length/angle display
- Undo/redo for line operations
- Line style options (dashed, dotted, etc.)
- Dimension tools integration

## Comparison to AutoCAD/Fusion 360

| Feature | AutoCAD/Fusion 360 | Our Implementation |
|---------|-------------------|-------------------|
| Standard Line | ✅ | ✅ |
| Ray | ✅ | ✅ |
| Construction Line | ✅ | ✅ |
| Polyline | ✅ | ✅ |
| Spline | ✅ | ✅ |
| Bezier | ✅ | ✅ |
| Tangent | ✅ | ✅ |
| Perpendicular | ✅ | ✅ |
| Parallel | ✅ | ✅ |
| Offset | ✅ | ✅ |
| Fillet | ✅ | ✅ |
| Chamfer | ✅ | ✅ |
| Trim | ✅ | ✅ |
| Extend | ✅ | ✅ |
| Break | ✅ | ✅ |
| Join | ✅ | ✅ |

All major line tools from AutoCAD/Fusion 360 are now available! 🎉

