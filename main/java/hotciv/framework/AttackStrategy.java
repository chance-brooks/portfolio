package hotciv.framework;

import hotciv.standard.GameImpl;
import hotciv.standard.UnitImpl;

public interface AttackStrategy {
    public boolean attackUnit(Position from, Position to, GameImpl game, boolean debug);
}
