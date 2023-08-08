@app.route('/delete', method='POST')
def delete_product():
    product_id = request.forms.get('id')
    product_key = f'product:{product_id}'
       
    # Check if the product exists
    if r.exists(product_key):
        # Retrieve the product data
        product = r.hgetall(product_key)
        
        # Add a "deleted" field to the product data
        deletion_date = "2023-08-07"  # Use the actual deletion date
        product[b'deleted'] = deletion_date
        
        # Store the deleted product in a separate hash
        deleted_key = f'deleted_product:{product_id}'
        r.hmset(deleted_key, product)
        
        # Remove the product from the main products hash
        r.delete(product_key)
        
        return 'Product marked as deleted.'
    else:
        return 'Product not found.'

@app.route('/history')
def history():
    # Retrieve all product keys, including deleted ones
    all_keys = r.keys('*')

    # Retrieve product data for each key
    all_products = []
    for key in all_keys:
        if key.startswith('product:') or key.startswith('deleted_product:'):
            product = r.hgetall(key)
            all_products.append(product)

    return template('history', products=all_products)

<!-- ... -->
<table id="productTable" class="display">
    <!-- ... -->
    <tbody>
        % for product in products:
        <tr>
            <!-- ... -->
            <td>
                % if not b'deleted' in product:
                    <button class="deleteButton"
                            data-id="${product[b'id'].decode()}"
                            data-name="${product[b'name'].decode()}"
                            data-description="${product[b'description'].decode()}">
                        Delete
                    </button>
                % endif
            </td>
        </tr>
        % endfor
    </tbody>
</table>
<!-- ... -->
<script>
    $(document).ready(function() {
        $('#productTable').DataTable();

        // Handle delete button clicks
        $('.deleteButton').on('click', function() {
            var productId = $(this).data('id');
            var productName = $(this).data('name');
            var productDescription = $(this).data('description');

            if (confirm('Are you sure you want to delete the product:\n' +
                        'Name: ' + productName + '\n' +
                        'Description: ' + productDescription)) {
                deleteProduct(productId, productName, productDescription);
            }
        });

        function deleteProduct(productId, productName, productDescription) {
            $.post('/delete', { id: productId, name: productName, description: productDescription }, function(data) {
                if (data === 'ok') {
                    alert('Product deleted successfully.');
                    // You may want to refresh the table or update the UI here
                } else {
                    alert('Error deleting product.');
                }
            });
        }
    });
</script>

