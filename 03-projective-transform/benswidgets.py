
import numpy as np
from matplotlib.lines import Line2D

class InteractiveQuadrilateral(object):
    
    _epsilon = 6  # max pixel distance to count as a vertex hit
    
    def __init__(self, axes, xdata, ydata):
        if len(xdata) != 4 or len(ydata) != 4:
            raise RuntimeError('You must have exactly four points to initialize a quadrilateral')
        self.axes = axes
        self.canvas = axes.figure.canvas
        self.xdata = xdata
        self.ydata = ydata
        
        self.line = Line2D(xdata = np.hstack((xdata, xdata[0])),
                           ydata = np.hstack((ydata, ydata[0])),
                           color='#f92672',   # Pink from molokai color scheme
                           marker='o',
                           markerfacecolor='#ffffff',
                           animated=True)
        
        self.axes.add_line(self.line)
        
        self.canvas.mpl_connect('draw_event', self._draw_callback)
        self.canvas.mpl_connect('button_press_event', self._button_press_callback)
        self.canvas.mpl_connect('button_release_event', self._button_release_callback)
        self.canvas.mpl_connect('motion_notify_event', self._motion_notify_callback)
        
        self._selected_vertex = None
        self._observers = []
        
    
    def on_changed(self, func):
        """
        Add func to the list of observers.
        The observers will be called whenever this widget changes.
        """
        self._observers.append(func)
        

    def _draw_callback(self, event):
        """This gets called when plt.show() is called or when the window is resized"""
        self.background = self.canvas.copy_from_bbox(self.axes.bbox)
        self.axes.draw_artist(self.line)
        self.canvas.blit(self.axes.bbox)
        

    def _button_press_callback(self, event):
        """This gets called when a mouse button is pressed"""
        if event.inaxes is None:
            return
        if event.button != 1:
            return
        
        # get the index of the vertex under point if within epsilon tolerance
        
        xy = np.array((self.xdata, self.ydata)).T       # image-coordinates of the vertices
        xyt = self.line.get_transform().transform(xy)
        xt, yt = xyt[:, 0], xyt[:, 1]                   # display-coordinates of the vertices
        d = np.sqrt((xt - event.x)**2 + (yt - event.y)**2)
        
        closest_vertex = np.argmin(d)

        if d[closest_vertex] < self._epsilon:
            self._selected_vertex = closest_vertex
        else:
            self._selected_vertex = None
        

    def _button_release_callback(self, event):
        """This gets called when a mouse button is released"""
        if event.button != 1:
            return
        self._selected_vertex = None


    def _motion_notify_callback(self, event):            
        """This gets called when the mouse moves"""
        if self._selected_vertex is None:
            return
        if event.inaxes is None:
            return
        if event.button != 1:
            return
        x, y = event.xdata, event.ydata

        self.xdata[self._selected_vertex] = x
        self.ydata[self._selected_vertex] = y
        
        self.line.set_xdata(np.hstack((self.xdata, self.xdata[0])))
        self.line.set_ydata(np.hstack((self.ydata, self.ydata[0])))

        self.canvas.restore_region(self.background)
        self.axes.draw_artist(self.line)
        self.canvas.blit(self.axes.bbox)
        
        # Call all observer functions
        for func in self._observers:
            func()
        
