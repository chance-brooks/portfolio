package hotciv.visual;

import minidraw.framework.Tool;
import minidraw.standard.*;
import java.awt.event.*;
import hotciv.framework.*;

public class MoveTool extends NullTool {
    private Game game;
    private int fromX, fromY;
    public boolean validMove;   //flag to determine if gui needs updated
    public MoveTool (Game game) {this.game = game;}

    public void mouseDown(MouseEvent e, int x, int y) {
        if (game.getUnitAt(new Position(x/16, y/16)) != null) {
            fromX = x/16;
            fromY = y/16;
            validMove = true;
        }
    }

    @Override
    public void mouseDrag(MouseEvent e, int x, int y) {

    }

    public void mouseUp(MouseEvent e, int x, int y) {
        if (validMove)
            validMove = game.moveUnit(new Position(fromX, fromY), new Position(x/16, y/16));
    }

    @Override
    public void mouseMove(MouseEvent e, int x, int y) {

    }

    @Override
    public void keyDown(KeyEvent e, int key) {

    }
}
