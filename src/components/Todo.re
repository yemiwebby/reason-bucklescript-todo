type state = {
  items: list(TodoType.item),
  inputText: string,
};

type action =
  | InputText(string)
  | Toggle(int)
  | RemoveItem(int)
  | Submit;

let component = ReasonReact.reducerComponent("Todo");

let make = _children => {
  let handleSubmit = state => {
    let newId: int = List.length(state.items);
    let newItem: TodoType.item = {
      id: newId,
      title: state.inputText,
      completed: false,
    };
    let newList = [newItem, ...state.items];
    ReasonReact.Update({items: newList, inputText: ""});
  };
  {
    ...component,
    initialState: () => {
      items: [{id: 0, title: "Fix more bugs", completed: false}],
      inputText: "",
    },
    reducer: action =>
      switch (action) {
      | InputText(newText) => (
          state => ReasonReact.Update({...state, inputText: newText})
        )
      | Toggle(id) => (state => ReasonReact.Update({
              ...state,
              items:
                List.map((item: TodoType.item) =>
                    item.id == id ?
                      {
                        ...item,
                        TodoType.completed: ! TodoType.(item.completed),
                      } :
                      item,
                  state.items,
                ),
            })
        )
      | RemoveItem(id) => ( state =>
            ReasonReact.Update({
              ...state,
              items:
                List.filter(
                  (item: TodoType.item) => item.id !== id,
                  state.items,
                ),
            })
        )
      | Submit => (state => handleSubmit(state))
      },
    render: self => {
      let {items, inputText} = self.state;
      <div className="app">
        <div className="app-header">
          <div className="title"> {ReasonReact.string("Todo List")} </div>
        </div>
        <Input
          submit=(_ => self.send(Submit))
          value=inputText
          onInputText=(text => self.send(InputText(text)))
        />
        <div className="list">
          (
            ReasonReact.array(
              Array.of_list(
                List.map(
                  (item: TodoType.item) =>
                    <Item
                      key=(string_of_int(item.id))
                      item
                      onRemove=(id => self.send(RemoveItem(id)))
                      onToggle=(id => self.send(Toggle(id)))
                    />,
                  items,
                ),
              ),
            )
          )
        </div>
      </div>;
    },
  };
};