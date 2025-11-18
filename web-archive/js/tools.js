/**
 * Drawing Tools Implementation
 */
class DrawingTools {
    constructor(drawingEngine) {
        this.engine = drawingEngine;
        this.currentTool = 'pen';
        this.startX = 0;
        this.startY = 0;
        this.isDrawing = false;
        this.tempCanvas = null;
        this.tempCtx = null;
        
        this.setupTempCanvas();
        this.setupToolListeners();
    }
    
    setupTempCanvas() {
        // Create temporary canvas for preview shapes
        this.tempCanvas = document.createElement('canvas');
        const dpr = window.devicePixelRatio || 1;
        this.tempCanvas.width = this.engine.canvas.width;
        this.tempCanvas.height = this.engine.canvas.height;
        this.tempCtx = this.tempCanvas.getContext('2d');
        this.tempCtx.scale(dpr, dpr);
        this.tempCanvas.style.position = 'absolute';
        const rect = this.engine.canvas.getBoundingClientRect();
        this.tempCanvas.style.top = rect.top + 'px';
        this.tempCanvas.style.left = rect.left + 'px';
        this.tempCanvas.style.width = rect.width + 'px';
        this.tempCanvas.style.height = rect.height + 'px';
        this.tempCanvas.style.pointerEvents = 'none';
        this.tempCanvas.style.zIndex = '10';
        this.engine.canvas.parentElement.appendChild(this.tempCanvas);
    }
    
    setupToolListeners() {
        // Override drawing engine events for shape tools
        const originalStart = this.engine.startDrawing.bind(this.engine);
        const originalDraw = this.engine.draw.bind(this.engine);
        const originalStop = this.engine.stopDrawing.bind(this.engine);
        
        this.engine.canvas.addEventListener('mousedown', (e) => {
            if (['line', 'rectangle', 'circle', 'arrow'].includes(this.currentTool)) {
                this.startShape(e);
            } else {
                originalStart(e);
            }
        });
        
        this.engine.canvas.addEventListener('mousemove', (e) => {
            if (this.isDrawing && ['line', 'rectangle', 'circle', 'arrow'].includes(this.currentTool)) {
                this.updateShape(e);
            } else if (!['line', 'rectangle', 'circle', 'arrow'].includes(this.currentTool)) {
                originalDraw(e);
            }
        });
        
        this.engine.canvas.addEventListener('mouseup', (e) => {
            if (this.isDrawing && ['line', 'rectangle', 'circle', 'arrow'].includes(this.currentTool)) {
                this.finishShape(e);
            } else {
                originalStop(e);
            }
        });
    }
    
    setTool(tool) {
        this.currentTool = tool;
        this.engine.canvas.style.cursor = this.getCursorForTool(tool);
    }
    
    getCursorForTool(tool) {
        const cursors = {
            'pen': 'crosshair',
            'pencil': 'crosshair',
            'brush': 'crosshair',
            'eraser': 'grab',
            'line': 'crosshair',
            'rectangle': 'crosshair',
            'circle': 'crosshair',
            'arrow': 'crosshair',
            'text': 'text',
            'ruler': 'crosshair',
            'protractor': 'crosshair'
        };
        return cursors[tool] || 'default';
    }
    
    startShape(e) {
        this.isDrawing = true;
        const coords = this.engine.getCoordinates(e);
        this.startX = coords.x;
        this.startY = coords.y;
        
        // Match temp canvas size to main canvas
        const rect = this.engine.canvas.getBoundingClientRect();
        const dpr = window.devicePixelRatio || 1;
        this.tempCanvas.width = this.engine.canvas.width;
        this.tempCanvas.height = this.engine.canvas.height;
        this.tempCanvas.style.width = rect.width + 'px';
        this.tempCanvas.style.height = rect.height + 'px';
        this.tempCanvas.style.top = rect.top + 'px';
        this.tempCanvas.style.left = rect.left + 'px';
        
        // Reset context and scale
        this.tempCtx.setTransform(1, 0, 0, 1, 0, 0);
        this.tempCtx.scale(dpr, dpr);
    }
    
    updateShape(e) {
        const coords = this.engine.getCoordinates(e);
        const endX = coords.x;
        const endY = coords.y;
        
        // Clear temp canvas
        this.tempCtx.clearRect(0, 0, this.tempCanvas.width, this.tempCanvas.height);
        
        // Draw preview
        this.tempCtx.strokeStyle = this.engine.color;
        this.tempCtx.lineWidth = this.engine.lineWidth;
        this.tempCtx.globalAlpha = this.engine.opacity;
        this.tempCtx.setLineDash([5, 5]);
        
        this.drawShapePreview(this.tempCtx, this.startX, this.startY, endX, endY);
        
        this.tempCtx.setLineDash([]);
    }
    
    drawShapePreview(ctx, x1, y1, x2, y2) {
        switch (this.currentTool) {
            case 'line':
                ctx.beginPath();
                ctx.moveTo(x1, y1);
                ctx.lineTo(x2, y2);
                ctx.stroke();
                break;
                
            case 'rectangle':
                const rectWidth = x2 - x1;
                const rectHeight = y2 - y1;
                ctx.strokeRect(x1, y1, rectWidth, rectHeight);
                break;
                
            case 'circle':
                const radius = Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
                ctx.beginPath();
                ctx.arc(x1, y1, radius, 0, Math.PI * 2);
                ctx.stroke();
                break;
                
            case 'arrow':
                this.drawArrow(ctx, x1, y1, x2, y2);
                break;
        }
    }
    
    drawArrow(ctx, x1, y1, x2, y2) {
        const headlen = 15;
        const angle = Math.atan2(y2 - y1, x2 - x1);
        
        ctx.beginPath();
        ctx.moveTo(x1, y1);
        ctx.lineTo(x2, y2);
        ctx.lineTo(x2 - headlen * Math.cos(angle - Math.PI / 6), y2 - headlen * Math.sin(angle - Math.PI / 6));
        ctx.moveTo(x2, y2);
        ctx.lineTo(x2 - headlen * Math.cos(angle + Math.PI / 6), y2 - headlen * Math.sin(angle + Math.PI / 6));
        ctx.stroke();
    }
    
    finishShape(e) {
        if (!this.isDrawing) return;
        
        const coords = this.engine.getCoordinates(e);
        const endX = coords.x;
        const endY = coords.y;
        
        // Clear temp canvas
        this.tempCtx.clearRect(0, 0, this.tempCanvas.width, this.tempCanvas.height);
        
        // Draw final shape on main canvas
        this.engine.ctx.strokeStyle = this.engine.color;
        this.engine.ctx.lineWidth = this.engine.lineWidth;
        this.engine.ctx.globalAlpha = this.engine.opacity;
        
        this.drawShapeFinal(this.engine.ctx, this.startX, this.startY, endX, endY);
        
        this.isDrawing = false;
    }
    
    drawShapeFinal(ctx, x1, y1, x2, y2) {
        switch (this.currentTool) {
            case 'line':
                ctx.beginPath();
                ctx.moveTo(x1, y1);
                ctx.lineTo(x2, y2);
                ctx.stroke();
                break;
                
            case 'rectangle':
                const rectWidth = x2 - x1;
                const rectHeight = y2 - y1;
                ctx.strokeRect(x1, y1, rectWidth, rectHeight);
                break;
                
            case 'circle':
                const radius = Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
                ctx.beginPath();
                ctx.arc(x1, y1, radius, 0, Math.PI * 2);
                ctx.stroke();
                break;
                
            case 'arrow':
                this.drawArrow(ctx, x1, y1, x2, y2);
                break;
        }
    }
    
    // Eraser tool
    erase(x, y, radius) {
        this.engine.ctx.globalCompositeOperation = 'destination-out';
        this.engine.ctx.beginPath();
        this.engine.ctx.arc(x, y, radius, 0, Math.PI * 2);
        this.engine.ctx.fill();
        this.engine.ctx.globalCompositeOperation = 'source-over';
    }
}

