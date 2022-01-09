package hotciv.standard;

import hotciv.framework.Game;
import hotciv.framework.GameConstants;
import hotciv.framework.Player;
import hotciv.framework.Position;
import junit.framework.TestCase;
import org.junit.Before;
import org.junit.Test;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.notNullValue;
import static org.junit.Assert.*;
import static org.junit.Assert.assertEquals;

public class GameTranscriptTest{
    Game game;
    @Before
    public void setUp() {
        game = new GameTranscriptImpl(new GameImpl(new AlphaCivFactory()));
    }

    /*@Test
    public void movePrintout(){
        Position from = new Position(4,4);
        Position to = new Position(3,3);

        ((GameImpl)game).createUnit(from, GameConstants.ARCHER, Player.RED);

        game.moveUnit(from, to);

        //Manually checked
    }*/

    @Test
    public void changeWorkforcePrintout(){
        game.changeWorkForceFocusInCityAt(new Position(1,1), GameConstants.productionFocus);
        game.changeWorkForceFocusInCityAt(new Position(1,1), GameConstants.foodFocus);

        //Manually checked
    }

    @Test
    public void changeProductionPrintout(){
        game.changeProductionInCityAt(new Position(1,1), GameConstants.ARCHER);
        game.changeProductionInCityAt(new Position(1,1), GameConstants.SETTLER);

        //Manually checked
    }

   /* @Test
    public void endOfTurnPrintout(){
        Position from = new Position(2,1);
        Position to = new Position(3,1);
        ((GameImpl)game).createUnit(from, GameConstants.ARCHER, Player.RED);
        game.moveUnit(from,to);

        game.endOfTurn();

        //Manually checked
    }*/
}