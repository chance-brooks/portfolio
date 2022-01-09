package hotciv.standard;

import hotciv.framework.GameObserver;
import hotciv.framework.Position;

public class GameTranscriptImpl extends GameTranscript {
    //GameImpl game = new GameImpl(new AlphaCivFactory());
    //GameImpl game;

    public GameTranscriptImpl(GameImpl g){
        super(g);
    }
    @Override
    public boolean moveUnit(Position from, Position to ){
        System.out.println(super.getPlayerInTurn() + " moves " + super.getUnitAt(from).getTypeString() + " from " + from + " to " + to + ".\n");
        return super.moveUnit(from, to);
    }

    @Override
    public void endOfTurn(){
        System.out.println(super.getPlayerInTurn() + " ends turn.\n");
        super.endOfTurn();
    }

    @Override
    public void changeWorkForceFocusInCityAt( Position p, String balance ){
        System.out.println(super.getPlayerInTurn() + " changes work focus in city at " + p + " to " + balance + ".\n");
        super.changeWorkForceFocusInCityAt(p, balance);
    }

    @Override
    public void changeProductionInCityAt( Position p, String unitType ){
        System.out.println(super.getPlayerInTurn() + " changes production in city at " + p + " to " + unitType + ".\n");
        super.changeProductionInCityAt(p, unitType);
    }

    @Override
    public void addObserver(GameObserver observer) {

    }

    @Override
    public void setTileFocus(Position position) {

    }

}
