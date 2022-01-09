package hotciv.standard;

import hotciv.framework.*;

public abstract class GameTranscript implements Game {
    private Game game;
    public GameTranscript(Game g){
        game = g;
    }

    public Tile getTileAt(Position p ){ return game.getTileAt(p);}

    public Unit getUnitAt(Position p ){ return game.getUnitAt(p);}

    public City getCityAt(Position p ){ return game.getCityAt(p);}

    public Player getPlayerInTurn(){ return game.getPlayerInTurn();}

    public Player getWinner(){ return game.getWinner();}

    public int getAge(){return game.getAge();}

    public boolean moveUnit( Position from, Position to ){
        //System.out.println(game.getPlayerInTurn() + " moves " + game.getUnitAt(from).getTypeString() + " from " + from + " to " + to + ".\n");
        return game.moveUnit(from, to);
    }

    public void endOfTurn(){
        //System.out.println(game.getPlayerInTurn() + " ends turn.\n");
        game.endOfTurn();
    }

    public void changeWorkForceFocusInCityAt( Position p, String balance ){
        //System.out.println(game.getPlayerInTurn() + " changes work focus in city at " + p + " to " + balance + ".\n");
        game.changeWorkForceFocusInCityAt(p, balance);
    }

    public void changeProductionInCityAt( Position p, String unitType ){
        //System.out.println(game.getPlayerInTurn() + " changes production in city at " + p + " to " + unitType + ".\n");
        game.changeProductionInCityAt(p, unitType);
    }
    //THIS NEEDS REMOVED TOO
    //public boolean createUnit(Position p, String kindOfUnit, Player ownerOfUnit){
    //    return game.createUnit(p, kindOfUnit, ownerOfUnit);
    //}




}
