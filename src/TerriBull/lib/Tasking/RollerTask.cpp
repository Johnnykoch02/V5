
 RollerTask(TerriBull::MechanicalSystem* _system);
    
    public:
    
    RollerTask(float time, int direction, TerriBull::MechanicalSystem* _system);

    RollerTask(float pos, TerriBull::MechanicalSystem* _system);

    static RollerTask* DynamicInitialize(float* pos, float offset, TerriBull::MechanicalSystem* _system);

    ~RollerTask();

    void init();
    void update(float delta); 