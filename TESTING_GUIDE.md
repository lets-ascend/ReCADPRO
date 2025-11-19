# 🧪 ReCADPro Testing Guide

## Quick Testing Checklist

Use this checklist to verify all features work correctly on your reMarkable Paper Pro.

---

## ✅ Basic Functionality Tests

### Launch & UI
- [ ] Application launches without errors
- [ ] Main window appears full screen
- [ ] Toolbar visible at top
- [ ] Sidebar visible on left
- [ ] Layers panel visible on right
- [ ] Drawing canvas fills center area
- [ ] No console errors on launch

### Drawing Tools
- [ ] **Pen Tool**: Draws smooth strokes
- [ ] **Pen Tool**: Pressure sensitivity works (thicker with more pressure)
- [ ] **Pencil Tool**: Draws with pencil-like texture
- [ ] **Brush Tool**: Variable-width strokes
- [ ] **Eraser**: Erases strokes correctly
- [ ] **Line Tool**: Draws straight lines
- [ ] **Rectangle Tool**: Draws rectangles
- [ ] **Circle Tool**: Draws circles
- [ ] **Arc Tool**: Draws arcs (test all 3 modes)

### Basic Operations
- [ ] **Undo**: Reverses last action (Ctrl+Z or swipe right)
- [ ] **Redo**: Re-applies undone action (Ctrl+Shift+Z or swipe left)
- [ ] **Clear**: Clears entire canvas
- [ ] **Save**: Saves project file
- [ ] **Load**: Loads project file

---

## ✅ Advanced Feature Tests

### Object Snap (OSNAP)
- [ ] Enable snap in sidebar
- [ ] **Endpoint Snap**: Snaps to line endpoints
- [ ] **Midpoint Snap**: Snaps to line midpoints
- [ ] **Intersection Snap**: Snaps to line intersections
- [ ] **Center Snap**: Snaps to circle/arc centers
- [ ] **Perpendicular Snap**: Snaps perpendicular to lines
- [ ] **Tangent Snap**: Snaps tangent to circles/arcs
- [ ] Snap indicator shows correct color
- [ ] Snap tolerance slider works

### Zoom & Pan
- [ ] **Pinch-to-zoom**: Zoom in/out with pinch gesture
- [ ] **Two-finger pan**: Pan canvas with two fingers
- [ ] **Zoom In Button**: Increases zoom level
- [ ] **Zoom Out Button**: Decreases zoom level
- [ ] **Zoom Slider**: Continuous zoom control
- [ ] **Zoom to Fit**: Fits entire canvas to view
- [ ] **Zoom to Selection**: Fits selection to view
- [ ] **Reset**: Resets zoom to 100%
- [ ] Zoom level display shows correct percentage

### Selection System
- [ ] **Rectangle Selection**: Select strokes with rectangle
- [ ] **Multi-select**: Select multiple strokes
- [ ] **Selection Bounds**: Bounding box shows correctly
- [ ] **Copy**: Copies selected strokes (Ctrl+C)
- [ ] **Paste**: Pastes copied strokes (Ctrl+V)
- [ ] **Cut**: Cuts selected strokes (Ctrl+X)
- [ ] **Delete**: Deletes selected strokes (Delete/Backspace)
- [ ] **Move**: Moves selected strokes

### Shape Tools
- [ ] **Rectangle**: Draws rectangles with fill option
- [ ] **Circle**: Draws circles with fill option
- [ ] **Line**: Draws straight lines
- [ ] **Arrow**: Draws arrows
- [ ] **Arc 3-Point**: Creates arc through 3 points
- [ ] **Arc Center-Radius**: Creates arc from center
- [ ] **Arc Start-End-Radius**: Creates arc with radius

### Advanced Line Tools
- [ ] **Polyline**: Draws connected line segments
- [ ] **Spline**: Draws smooth curves
- [ ] **Bezier**: Draws Bezier curves
- [ ] **Construction Line**: Draws infinite construction lines
- [ ] **Ray**: Draws rays
- [ ] **Fillet**: Rounds corners
- [ ] **Chamfer**: Bevels corners
- [ ] **Trim**: Trims lines
- [ ] **Extend**: Extends lines
- [ ] **Offset**: Offsets lines
- [ ] **Break**: Breaks lines
- [ ] **Join**: Joins lines

### Text Tool
- [ ] **Place Text**: Places text on canvas
- [ ] **Text Input Dialog**: Opens when clicking canvas
- [ ] **Font Selection**: Changes font
- [ ] **Text Color**: Changes text color
- [ ] **Edit Text**: Edits existing text
- [ ] **Delete Text**: Deletes text objects
- [ ] **Multiple Text Objects**: Multiple texts on canvas

### Measurement Tools
- [ ] **Ruler Tool**: Measures distance
- [ ] **Ruler Display**: Shows measurement value
- [ ] **Protractor Tool**: Measures angles
- [ ] **Protractor Display**: Shows angle value
- [ ] **Dimension Lines**: Shows dimension annotations
- [ ] **Area Measurement**: Measures polygon area
- [ ] **Area Fill**: Visualizes measured area

### Layer System
- [ ] **Add Layer**: Creates new layer
- [ ] **Delete Layer**: Removes layer
- [ ] **Rename Layer**: Changes layer name
- [ ] **Show/Hide Layer**: Toggles visibility
- [ ] **Layer Opacity**: Adjusts opacity slider
- [ ] **Layer Blend Modes**: Changes blend mode
- [ ] **Active Layer**: Switches active layer
- [ ] **Layer Order**: Reorders layers

### Export Functions
- [ ] **PNG Export**: Exports to PNG format
- [ ] **SVG Export**: Exports to SVG format
- [ ] **PDF Export**: Exports to PDF format
- [ ] **Export Dialog**: Opens export options
- [ ] **Export Quality**: Adjusts quality settings

### Grid & Drafting
- [ ] **Grid Toggle**: Shows/hides grid (G key)
- [ ] **Grid Type**: Switches grid type (lines/dots/isometric)
- [ ] **Grid Size**: Adjusts grid size
- [ ] **Snap to Grid**: Snaps points to grid
- [ ] **Grid Overlay**: Grid displays correctly

### 3D Tools
- [ ] **Isometric Box**: Creates isometric box
- [ ] **Isometric Cylinder**: Creates isometric cylinder
- [ ] **3D Primitives**: Creates sphere, cone, pyramid, torus
- [ ] **3D Rotation**: Rotates 3D objects
- [ ] **Isometric Views**: Switches isometric views
- [ ] **3D Projection**: Projects 3D to 2D

---

## ✅ Performance Tests

### Drawing Performance
- [ ] **Smooth Drawing**: No lag when drawing
- [ ] **Fast Stroke Rendering**: Strokes appear immediately
- [ ] **Pressure Response**: Pressure changes respond quickly
- [ ] **Large Canvas**: Handles large drawings without slowdown

### UI Performance
- [ ] **Responsive UI**: Buttons respond immediately
- [ ] **Smooth Scrolling**: Sidebar scrolls smoothly
- [ ] **Fast Tool Switching**: Tools switch instantly
- [ ] **Quick Layer Operations**: Layer changes are fast

### E-paper Optimization
- [ ] **Refresh Rate**: Screen refreshes appropriately
- [ ] **Partial Updates**: Only changed areas refresh
- [ ] **No Flicker**: Screen doesn't flicker excessively
- [ ] **Clear Display**: Text and UI are readable

---

## ✅ Edge Cases & Error Handling

### Error Handling
- [ ] **Invalid File**: Handles invalid project files gracefully
- [ ] **Missing Resources**: Handles missing icons gracefully
- [ ] **Large Files**: Handles large project files
- [ ] **Memory**: Doesn't crash with many strokes
- [ ] **Network**: Handles device disconnection gracefully

### Edge Cases
- [ ] **Empty Canvas**: Operations work on empty canvas
- [ ] **Single Stroke**: Operations work with single stroke
- [ ] **Many Strokes**: Operations work with many strokes
- [ ] **Overlapping Shapes**: Handles overlapping correctly
- [ ] **Zoom Limits**: Respects min/max zoom limits

---

## 🐛 Bug Reporting Template

If you find bugs, report them with:

```
**Bug Description**: [Brief description]

**Steps to Reproduce**:
1. [Step 1]
2. [Step 2]
3. [Step 3]

**Expected Behavior**: [What should happen]

**Actual Behavior**: [What actually happens]

**Device Info**:
- Device Version: [e.g., 3.10.5]
- App Version: [e.g., 1.0.0]

**Screenshots/Logs**: [If available]
```

---

## ✅ Success Criteria

Your testing is complete when:
- ✅ All basic functionality tests pass
- ✅ All advanced feature tests pass
- ✅ Performance is acceptable
- ✅ No critical bugs found
- ✅ App is ready for daily use

---

## 📝 Testing Notes

Document any issues, observations, or suggestions here:

```
[Date] - [Test] - [Result] - [Notes]
```

---

**Happy Testing! 🎉**

