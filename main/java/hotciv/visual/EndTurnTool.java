package hotciv.visual;

import minidraw.standard.*;
import java.awt.event.*;
import hotciv.framework.*;

public class EndTurnTool extends NullTool{
    private Game game;
    public EndTurnTool (Game game) {this.game = game;}

    public void mouseDown(MouseEvent e, int x, int y) {
        if (x>=550 && x<=590 && y>=60 && y<=105)
            game.endOfTurn();
    }
}

