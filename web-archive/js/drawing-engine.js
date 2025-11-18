/**
 * Advanced Drawing Engine with pressure sensitivity and smooth strokes
 */
class DrawingEngine {
    constructor(canvasId) {
        this.canvas = document.getElementById(canvasId);
        this.ctx = this.canvas.getContext('2d', { 
            willReadFrequently: true,
            desynchronized: true 
        });
        
        this.isDrawing = false;
        this.lastX = 0;
        this.lastY = 0;
        this.currentPath = [];
        this.pressure = 1.0;
        
        // Drawing settings
        this.lineWidth = 3;
        this.color = '#000000';
        this.opacity = 1.0;
        this.pressureSensitive = true;
        
        // Smoothing
        this.smoothing = 0.5;
        this.points = [];
        
        this.setupCanvas();
        this.setupEventListeners();
    }
    
    setupCanvas() {
        const dpr = window.devicePixelRatio || 1;
        const rect = this.canvas.getBoundingClientRect();
        
        this.canvas.width = rect.width * dpr;
        this.canvas.height = rect.height * dpr;
        
        this.ctx.scale(dpr, dpr);
        this.ctx.lineCap = 'round';
        this.ctx.lineJoin = 'round';
        this.ctx.globalAlpha = this.opacity;
        
        // Set initial canvas size
        this.resizeCanvas(1200, 1600);
    }
    
    resizeCanvas(width, height) {
        const dpr = window.devicePixelRatio || 1;
        this.canvas.width = width * dpr;
        this.canvas.height = height * dpr;
        this.canvas.style.width = width + 'px';
        this.canvas.style.height = height + 'px';
        this.ctx.scale(dpr, dpr);
        this.ctx.lineCap = 'round';
        this.ctx.lineJoin = 'round';
    }
    
    setupEventListeners() {
        // Mouse events
        this.canvas.addEventListener('mousedown', (e) => this.startDrawing(e));
        this.canvas.addEventListener('mousemove', (e) => this.draw(e));
        this.canvas.addEventListener('mouseup', () => this.stopDrawing());
        this.canvas.addEventListener('mouseout', () => this.stopDrawing());
        
        // Touch events
        this.canvas.addEventListener('touchstart', (e) => {
            e.preventDefault();
            const touch = e.touches[0];
            const mouseEvent = new MouseEvent('mousedown', {
                clientX: touch.clientX,
                clientY: touch.clientY
            });
            this.canvas.dispatchEvent(mouseEvent);
        });
        
        this.canvas.addEventListener('touchmove', (e) => {
            e.preventDefault();
            const touch = e.touches[0];
            const mouseEvent = new MouseEvent('mousemove', {
                clientX: touch.clientX,
                clientY: touch.clientY
            });
            this.canvas.dispatchEvent(mouseEvent);
        });
        
        this.canvas.addEventListener('touchend', (e) => {
            e.preventDefault();
            const mouseEvent = new MouseEvent('mouseup', {});
            this.canvas.dispatchEvent(mouseEvent);
        });
        
        // Pointer events for pressure sensitivity
        this.canvas.addEventListener('pointerdown', (e) => {
            this.pressure = e.pressure || 1.0;
            this.startDrawing(e);
        });
        
        this.canvas.addEventListener('pointermove', (e) => {
            this.pressure = e.pressure || 1.0;
            this.draw(e);
        });
        
        this.canvas.addEventListener('pointerup', () => {
            this.stopDrawing();
        });
    }
    
    getCoordinates(e) {
        const rect = this.canvas.getBoundingClientRect();
        const dpr = window.devicePixelRatio || 1;
        
        // Account for CSS scaling and device pixel ratio
        const scaleX = (this.canvas.width / dpr) / rect.width;
        const scaleY = (this.canvas.height / dpr) / rect.height;
        
        return {
            x: (e.clientX - rect.left) * scaleX,
            y: (e.clientY - rect.top) * scaleY
        };
    }
    
    startDrawing(e) {
        this.isDrawing = true;
        const coords = this.getCoordinates(e);
        this.lastX = coords.x;
        this.lastY = coords.y;
        this.currentPath = [{ x: this.lastX, y: this.lastY, pressure: this.pressure }];
        this.points = [{ x: this.lastX, y: this.lastY, pressure: this.pressure }];
    }
    
    draw(e) {
        if (!this.isDrawing) return;
        
        const coords = this.getCoordinates(e);
        const currentX = coords.x;
        const currentY = coords.y;
        
        // Add point to path
        this.currentPath.push({ x: currentX, y: currentY, pressure: this.pressure });
        this.points.push({ x: currentX, y: currentY, pressure: this.pressure });
        
        // Limit points for performance
        if (this.points.length > 100) {
            this.points.shift();
        }
        
        // Draw smooth curve
        this.drawSmoothPath();
        
        this.lastX = currentX;
        this.lastY = currentY;
    }
    
    drawSmoothPath() {
        if (this.points.length < 2) return;
        
        this.ctx.strokeStyle = this.color;
        this.ctx.globalAlpha = this.opacity;
        
        // Use quadratic curves for smooth lines
        for (let i = 1; i < this.points.length; i++) {
            const prev = this.points[i - 1];
            const curr = this.points[i];
            const next = this.points[i + 1] || curr;
            
            const midX = (prev.x + curr.x) / 2;
            const midY = (prev.y + curr.y) / 2;
            
            const width = this.pressureSensitive 
                ? this.lineWidth * (0.5 + curr.pressure * 0.5)
                : this.lineWidth;
            
            this.ctx.lineWidth = width;
            
            if (i === 1) {
                this.ctx.beginPath();
                this.ctx.moveTo(prev.x, prev.y);
            }
            
            if (next) {
                this.ctx.quadraticCurveTo(curr.x, curr.y, midX, midY);
            } else {
                this.ctx.lineTo(curr.x, curr.y);
            }
            
            this.ctx.stroke();
        }
    }
    
    stopDrawing() {
        if (!this.isDrawing) return;
        
        this.isDrawing = false;
        this.currentPath = [];
        this.points = [];
    }
    
    clear() {
        this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
    }
    
    setLineWidth(width) {
        this.lineWidth = width;
    }
    
    setColor(color) {
        this.color = color;
    }
    
    setOpacity(opacity) {
        this.opacity = opacity / 100;
        this.ctx.globalAlpha = this.opacity;
    }
    
    setPressureSensitive(enabled) {
        this.pressureSensitive = enabled;
    }
    
    getImageData() {
        return this.canvas.toDataURL('image/png');
    }
    
    setImageData(dataUrl) {
        const img = new Image();
        img.onload = () => {
            this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
            this.ctx.drawImage(img, 0, 0);
        };
        img.src = dataUrl;
    }
    
    exportSVG() {
        // Convert canvas to SVG
        const svg = document.createElementNS('http://www.w3.org/2000/svg', 'svg');
        svg.setAttribute('width', this.canvas.width);
        svg.setAttribute('height', this.canvas.height);
        
        const image = document.createElementNS('http://www.w3.org/2000/svg', 'image');
        image.setAttribute('href', this.canvas.toDataURL());
        image.setAttribute('width', this.canvas.width);
        image.setAttribute('height', this.canvas.height);
        
        svg.appendChild(image);
        
        return new XMLSerializer().serializeToString(svg);
    }
}

