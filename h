<!-- ... (head section of your HTML template) -->
<head>
    <title>Product List</title>
    <link rel="stylesheet" type="text/css" href="https://cdn.datatables.net/1.11.5/css/jquery.dataTables.min.css">
    <script type="text/javascript" charset="utf8" src="https://cdn.datatables.net/1.11.5/js/jquery.dataTables.min.js"></script>
    <script>
        $(document).ready( function () {
            $('#productTable').DataTable();
        } );
    </script>
</head>
<!-- ... -->

<!-- ... -->
<table id="productTable">
    <thead>
        <tr>
            <th>ID</th>
            <th>Name</th>
            <th>Description</th>
            <th>Price</th>
            <th>Action</th>
        </tr>
    </thead>
    <tbody>
        % for product in products:
        <tr>
            <td>{{ product.id }}</td>
            <td>{{ product.name }}</td>
            <td>{{ product.description }}</td>
            <td>{{ product.price }}</td>
            <td>
                % if product.deleted:
                    <span style="color: red;">Deleted on {{ product.deleted }}</span>
                % else:
                    <form method="post" action="/delete">
                        <input type="hidden" name="id" value="{{ product.id }}">
                        <button type="submit">Delete</button>
                    </form>
                % end
            </td>
        </tr>
        % end
    </tbody>
</table>
<!-- ... -->


from bottle import Bottle, run, template, request
import redis

app = Bottle()
r = redis.Redis(host='localhost', port=6379, db=0)

@app.route('/')
def index():
    # Retrieve product keys
    product_keys = r.keys('product:*')

    # Retrieve product data for each key
    products = []
    for key in product_keys:
        product = r.hgetall(key)
        products.append(product)

    return template('index', products=products)

# ... (previous code)

@app.route('/delete', method='POST')
def delete_product():
    product_id = request.forms.get('id')
    product_key = f'product:{product_id}'
    
    # Check if the product exists before marking as deleted
    if r.exists(product_key):
        # Update the product key and add "deleted" field to the hash
        deletion_date = "2023-08-07"  # Use the actual deletion date
        deleted_key = f'deleted:{product_key}'
        r.rename(product_key, deleted_key)
        r.hset(deleted_key, 'deleted', deletion_date)
        return 'Product marked as deleted.'
    else:
        return 'Product not found.'

# ... (previous code)


if __name__ == '__main__':
    run(app, debug=True)
