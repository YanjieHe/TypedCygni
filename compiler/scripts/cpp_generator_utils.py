def generate_fields_and_constructor(field_json):
    fields = []
    parameters = []
    initializers = []
    for field_type, field_name in field_json:
        fields.append(field_type + " " + field_name + ";")
        parameters.append(field_type + " " + field_name)
        initializers.append(field_name + "{" + field_name + "}")
    return (fields, parameters, initializers)